#include "Network/server.hpp"
#include "DataStructures/data_buffer.hpp"
#include "Network/message.hpp"
#include "Threading/thread_safe_iostream.hpp"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <ostream>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define ERROR -1

Server::Server(): _state(ConnectionState::STOPPED), _client_id_counter(1)
{
	// create a socket fd
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == ERROR)
		throw std::runtime_error("Unable to create socket!");

	// create a epoll instance
	_epollInstance = epoll_create(1);
	if (_epollInstance == ERROR)
		throw std::runtime_error("Couldn't create epoll instance error: " + std::string(std::strerror(errno)));

	// we only want to listen if we have incoming client connection (aka EPOLLIN)
	_event.events = EPOLLIN;
	_event.data.fd = _serverSocket;

	// add the server socket to the epoll instance to see if we have incoming clients connection we need to accept
	if (epoll_ctl(_epollInstance, EPOLL_CTL_ADD, _serverSocket, &_event) == ERROR)
		throw std::runtime_error("Couldn't add server socket to epoll instance error: " + std::string(std::strerror(errno)));
}

Server::~Server()
{
	close(_serverSocket);
	close(_epollInstance);

	for (auto& [clientID, clientFD] : _clients)
	{
		disconnectClient(clientID);
	}
}

void Server::start(const size_t& p_port)
{
	if (_state == RUNNING)
		throw std::runtime_error("Socket already Running!");

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(p_port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// bind to the server socket
	if (bind(_serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == ERROR)
		throw std::runtime_error("Couldn't bind to server socket error: " + std::string(std::strerror(errno)));

	// start listening to client connection
	// with a max queue of CLIENT_QUEUE_SIZE
	if (listen(_serverSocket, CLIENT_QUEUE_SIZE) == ERROR)
		throw std::runtime_error("Couldn't listen to server socket error: " + std::string(std::strerror(errno)));
	
	// transition the state to connected
	_state = RUNNING;
}

void Server::sendToAll(const Message& message)
{
	// iterate through all our active clients
	for (auto& [clientID, clientFD] : _clients)
	{
		sendTo(message, clientID);
	}
}


void Server::sendToArray(const Message& message, std::vector<long long> clientIDs)
{
	// iterate through the vector and send the message 1 to 1
	for (auto& clientID : clientIDs)
		sendTo(message, clientID);
}


void Server::sendTo(const Message& message, long long clientID)
{
	if (_state == STOPPED)
		throw std::runtime_error("Cannot send message! The socket is disconnected!");

	auto it = _clients.find(clientID);
	if (it == _clients.end())
		throw std::runtime_error("Sending to unkown client! clientID: " + std::to_string(clientID));

	PacketHeader header = constructPacketHeader(message);
	// send the header
	::send(it->second, &header, sizeof(header), 0);

	// send the data
	::send(it->second, message.data(), message.dataSize(), 0);
}

void Server::defineAction(const Message::Type& messageType, const ServerAction& action)
{
	auto it = _actions.find(messageType);

	if (it != _actions.end())
	{
		it->second.push_back(action);
	} else {
		auto it = _actions.emplace(messageType, std::vector<ServerAction>());
		
		// add the action to the vector
		it.first->second.push_back(action);
	}
}

void Server::update()
{
	if (_state == STOPPED)
		return;


	int activeEvents = epoll_wait(_epollInstance, _incomingEvents, MAX_EVENTS, 0);
	if (activeEvents == ERROR)
		throw std::runtime_error("Failed to wait for an active event reason " + std::string(std::strerror(errno)));

	// iterate through all requests
	for (int i = 0; i < activeEvents; i++)
	{
		// the client socket fd
		ClientFD client = _incomingEvents[i].data.fd;

		// if we have an event from the server socket we can
		if (client == _serverSocket)
		{
			connectClient();
			continue;
		}

		// did the client send us data?
		if (_incomingEvents[i].events & EPOLLIN)
		{
			readFromClient(client);
			continue;
		}

		// does the client want to disconnect from the server
		if (_incomingEvents[i].events & EPOLLERR & EPOLLHUP)
		{
			disconnectClient(client);
			continue;
		}
	}
}

void Server::readFromClient(SocketFD client)
{
	threadSafeCout << "reading from client " << std::endl;
	
	PacketHeader header = readHeaderFromClient(client);
	Message message = readDataFromClient(client, ntohl(header.dataSize), ntohl(header.Messagetype));

	executeMessageAction(client, message);
}

Server::PacketHeader Server::readHeaderFromClient(SocketFD client)
{
	Server::PacketHeader header;

	int bytesRead = recv(client, &header, sizeof(header), 0);
	if (bytesRead == ERROR)
	{
		disconnectClient(client);
		return header;
	}
	else if (bytesRead < static_cast<int>(sizeof(header)))
	{
		disconnectClient(client);
		
		// return early since we don't have to do anything here anymore
		throw std::runtime_error("Failed to read from client not enough to readd:  " + std::string(std::strerror(errno)));
	}
	return header;
}

Message Server::readDataFromClient(SocketFD client, uint32_t packetSize, Message::Type type)
{
	char rawBuffer[BUFFER_SIZE];

	int bytesRead = recv(client, &rawBuffer, packetSize, 0);
	if (bytesRead == ERROR)
	{
		disconnectClient(client);
		throw std::runtime_error("Failed to read from client socket reason: " + std::string(std::strerror(errno)));
	}

	else if (bytesRead < static_cast<int>(packetSize))
	{
		disconnectClient(client);
		// return early since we don't have to do anything here anymore
		throw std::runtime_error("Failed to read from client not enough to read:  " + std::string(std::strerror(errno)));
	}

	DataBuffer buffer(rawBuffer, packetSize);
	Message msg(type);

	msg << buffer;
	return msg;
}

void Server::executeMessageAction(ClientFD client, const Message& message)
{
	// do we have a action for this message type
	auto it = _actions.find(message.type());
	if (it == _actions.end())
		return;

	// find the clientID
	auto client_it = std::find_if(_clients.begin(), _clients.end(), [&](const auto& pair) {
		return pair.second == client;
	});

	// execute all actions
	for (auto& action : it->second)
	{
		long long client = client_it->first;
		action(client, message);
	}
}

void Server::connectClient()
{
	threadSafeCout << "Connecting Client!" << std::endl;
	int clientSocket = accept(_serverSocket, nullptr, nullptr);
	if (clientSocket == ERROR)
		throw std::runtime_error("Failed to connect to a client reason: " + std::string(std::strerror(errno)));

	_event.events = EPOLLIN;
	_event.data.fd = clientSocket;
	if (epoll_ctl(_epollInstance, EPOLL_CTL_ADD, clientSocket, &_event))
		throw std::runtime_error("Couldn't add client socket to epoll instance error: " + std::string(std::strerror(errno)));

	// mark the socket as non blocking
	int flags = fcntl(clientSocket, F_GETFL, 0);
	fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);

	// register the client
	_clients.emplace(generateUniqueIdentifier(), clientSocket);
}

void Server::disconnectClient(SocketFD client)
{
	// close our end of the socket
	close(client);
	
	// remove in our active clients
	std::erase_if(_clients, [&](const auto& pair) {
		return pair.second == client;
	});
}

long long Server::generateUniqueIdentifier()
{
	return _client_id_counter++;
}

Server::PacketHeader Server::constructPacketHeader(const Message& message)
{
	Server::PacketHeader header;

	header.dataSize = htonl(message.dataSize());
	header.Messagetype = htonl(message.type());

	return header;
}