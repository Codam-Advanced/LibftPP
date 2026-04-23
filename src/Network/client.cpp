#include "Network/client.hpp"
#include "Network/message.hpp"
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
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

	// send the data
	::send(_clientSocket, message.data(), message.dataSize(), 0);
}

void Client::disconnect()
{
	// if we are already disconnected just return no need to throw
	if (DISCONNECTED)
		return;

	// disconnect simply close the file descriptor
	close(_clientSocket);
}