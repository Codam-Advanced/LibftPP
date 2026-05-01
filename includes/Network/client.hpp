#pragma once

#include "Network/message.hpp"
#include "Network/server.hpp"
#include <functional>
#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 2068
class Client
{
	public:
		enum ConnectionState {
			DISCONNECTED,
			CONNECTED,
		};

		Client();
		~Client();

		void connect(const std::string& address, const size_t& port);
		void disconnect();
		void defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action);
		void send(const Message& message);
		void update();

	private:
		

		Server::PacketHeader readHeaderFromServer();
		Message readDataFromServer(uint32_t packetSize, Message::Type type);

		void executeMessageAction(const Message& message);

		int _clientSocket;
		ConnectionState _state;
		std::map<Message::Type, std::vector<std::function<void(const Message& msg)>>> _actions;
};