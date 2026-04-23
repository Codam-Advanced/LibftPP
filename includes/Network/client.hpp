#pragma once

#include "Network/message.hpp"
#include <functional>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Client
{
	public:
		enum ConnectionState {
			DISCONNECTED,
			CONNECTED,
		};

		Client();

		void connect(const std::string& address, const size_t& port);
		void disconnect();
		void defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action);
		void send(const Message& message);
		void update();

	private:
		int _clientSocket;
		ConnectionState _state;
};