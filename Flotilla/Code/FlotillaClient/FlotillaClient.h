#pragma once
#include "Address.h"
#include "Packet.h"
#include "Socket.h"
class FlotillaClient
{
private:
	Address serverAddress;
public:
	Socket clientSocket;
	FlotillaClient(Address server);
	~FlotillaClient();
	bool FlotillaClient::send(const Packet * const packet) const;

};

