#pragma once

#include "Network/message.hpp"
#include <functional>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>



#define BUFFER_SIZE 2068
#define CLIENT_QUEUE_SIZE 5
#define MAX_EVENTS 10
class Server
{
	using ServerAction = std::function<void(long long& clientID, const Message& msg)>;
	using SocketFD = int;
	using ClientFD = int;
	using EpollFD = int;


	public:

		struct PacketHeader 
		{
			uint32_t dataSize;   // size of payload only
			uint32_t Messagetype;   // message type 
		};

		enum ConnectionState {
			RUNNING,
			STOPPED,
		};

		Server();
		~Server();

		void start(const size_t& p_port);
		void defineAction(const Message::Type& messageType, const ServerAction& action);
		void sendTo(const Message& message, long long clientID);
		void sendToArray(const Message& message, std::vector<long long> clientIDs);
		void sendToAll(const Message& message);
		void update();

		static PacketHeader constructPacketHeader(const Message& message);

	private:

		void connectClient();
		void disconnectClient(SocketFD client);
		void readFromClient(SocketFD client);
		
		PacketHeader readHeaderFromClient(SocketFD client);
		Message readDataFromClient(SocketFD client, uint32_t packetSize, Message::Type type);

		long long generateUniqueIdentifier();

		void executeMessageAction(SocketFD client, const Message& message);

		SocketFD _serverSocket;
		EpollFD _epollInstance;

		struct epoll_event _event;
		struct epoll_event _incomingEvents[MAX_EVENTS];

		ConnectionState _state;
		std::map<long long, ClientFD> _clients;
		long long _client_id_counter;

		std::map<Message::Type, std::vector<ServerAction>> _actions;
};