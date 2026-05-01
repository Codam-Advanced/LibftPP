#include "Network/client.hpp"
#include "DataStructures/data_buffer.hpp"
#include "Network/message.hpp"
#include "Network/server.hpp"
#include "Threading/thread_safe_iostream.hpp"
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define ERROR -1

Client::Client(): _state(ConnectionState::DISCONNECTED)
{
	_clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (_clientSocket == ERROR)
		throw std::runtime_error("Unable to create socket!");
}

Client::~Client()
{
	if (_state == CONNECTED)
		disconnect();
}

void Client::connect(const std::string& address, const size_t& port)
{
	if (_state == CONNECTED)
		throw std::runtime_error("Socket already connected!");

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);

	// parse the ip address
	int result = inet_pton(AF_INET, address.c_str(), &serverAddress.sin_addr);
	if (result == 0 || result == ERROR)
		throw std::runtime_error("Couldn't convert ip address: " + address + " Error: " + std::string(std::strerror(errno)));

	// connect to the server
	int connected = ::connect(_clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if (connected == ERROR)
		throw std::runtime_error("Couldn't connect to server error: " + std::string(std::strerror(errno)));

	// transition the state to connected
	_state = CONNECTED;
}

void Client::send(const Message& message)
{
	if (_state == DISCONNECTED)
		throw std::runtime_error("Cannot send message! The socket is disconnected!");

	Server::PacketHeader header = Server::constructPacketHeader(message);
	// send the header
	::send(_clientSocket, &header, sizeof(header), 0);

	// send the data
	::send(_clientSocket, message.data(), message.dataSize(), 0);
}



void Client::defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action)
{
	auto it = _actions.find(messageType);

	if (it != _actions.end())
	{
		it->second.push_back(action);
	} else {
		auto it = _actions.emplace(messageType, std::vector<std::function<void(const Message& msg)>>());
		
		// add the action to the vector
		it.first->second.push_back(action);
	}
}

void Client::update()
{
	if (_state == DISCONNECTED)
		return;

	Server::PacketHeader header = readHeaderFromServer();
	Message message = readDataFromServer(ntohl(header.dataSize), ntohl(header.Messagetype));

	executeMessageAction(message);
}

Server::PacketHeader Client::readHeaderFromServer()
{
	Server::PacketHeader header;

	int bytesRead = recv(_clientSocket, &header, sizeof(header), 0);
	if (bytesRead == ERROR)
	{
		disconnect();
		throw std::runtime_error("Failed to read from client socket reason: " + std::string(std::strerror(errno)));
	}
	else if (bytesRead == 0)
	{
		// should we throw here?
		threadSafeCout << "Server disconnected" << std::endl;
		
		// disconnect the client socket
		disconnect();
		
		// return early since we don't have to do anything here anymore
		throw std::runtime_error("Failed to read from client nothing to read:  " + std::string(std::strerror(errno)));
	}
	return header;
}

Message Client::readDataFromServer(uint32_t packetSize, Message::Type type)
{
	char rawBuffer[BUFFER_SIZE];

	int bytesRead = recv(_clientSocket, &rawBuffer, packetSize, 0);
	if (bytesRead == ERROR)
	{
		disconnect();
		throw std::runtime_error("Failed to read from client socket reason: " + std::string(std::strerror(errno)));
	}
	else if (bytesRead == 0)
	{
		// should we throw here?
		threadSafeCout << "Server disconnected" << std::endl;
		
		// disconnect the client socket
		disconnect();
		
		// return early since we don't have to do anything here anymore
		throw std::runtime_error("Failed to read from client nothing to read:  " + std::string(std::strerror(errno)));
	}

	DataBuffer buffer(rawBuffer, packetSize);
	Message msg(type);

	msg << buffer;
	return msg;
}



void Client::executeMessageAction(const Message& message)
{
	auto it = _actions.find(message.type());
	
	if (it == _actions.end())
		return;

	for (auto& action : it->second)
	{
		action(message);
	}
}

void Client::disconnect()
{
	// if we are already disconnected just return no need to throw
	if (_state == DISCONNECTED)
		return;

	// disconnect simply close the file descriptor
	close(_clientSocket);

	// mark the client as disconnected
	_state = DISCONNECTED;
}