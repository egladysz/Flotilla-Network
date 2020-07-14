#pragma once
#include "Address.h"
#include "Socket.h"
#include "Packet.h"
constexpr int maxClientSlots = 8;

class Server
{

private:
	int connectedClientCount;
	int maxClients;
	bool clientConnected[maxClientSlots];
	Address clientAddress[maxClientSlots];
public:
	Socket socket;
	Server(unsigned short port);
	int findOpenSlotIndex() const;
	int findClientSlotIndex(const Address& address) const;
	bool isClientConnected(int index) const;
	const Address& getClientAddress(int index) const;
	int addClient(const Address& address);
	int removeClient(const Address& address);
	bool send(const Address& address,const Packet* const packet) const;
};

