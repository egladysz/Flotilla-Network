#include "Server.h"


Server::Server(unsigned short port)
{
	socket.open(port);
	maxClients = maxClientSlots;
	for (int i = 0; i < maxClients; i++)
	{
		clientConnected[i] = false;
	}
}

int Server::findOpenSlotIndex() const
{
	for (int i = 0; i < maxClients; ++i)
	{
		if (!clientConnected[i])
		{
			return i;
		}
	}
	return -1;
}

int Server::findClientSlotIndex(const Address & address) const
{
	for (int i = 0; i < maxClients; ++i)
	{
		if (clientConnected[i] && clientAddress[i] == address)
			return i;
	}
	return -1;
}

const Address & Server::getClientAddress(int index) const
{
	return clientAddress[index];
}

bool Server::isClientConnected(int index) const
{
	return clientConnected[index];
}

int Server::addClient(const Address & address)
{
	if (findClientSlotIndex(address) >= 0)
	{
		// already in
		return findClientSlotIndex(address);
	}
	int insertIndex = findOpenSlotIndex();
	if (insertIndex < 0)
	{
		//no room
		return insertIndex;
	}

	clientConnected[insertIndex] = true;
	clientAddress[insertIndex] = address;
	connectedClientCount += 1;
	return insertIndex;
}

int Server::removeClient(const Address & address)
{
	int clientIndex = findClientSlotIndex(address);

	if (clientIndex < 0)
	{
		//not here
		return clientIndex;
	}

	clientConnected[clientIndex] = false;
	clientAddress[clientIndex] = Address();
	connectedClientCount -= 1;

	
	return clientIndex;
}

bool Server::send(const Address & address, const Packet * const packet) const
{
	static uint8_t replyBufferArray[1536];
	Buffer replyBuffer;
	replyBuffer.data = replyBufferArray;
	replyBuffer.size = sizeof(replyBufferArray);
	replyBuffer.index = 0;
	packet->Write(replyBuffer);
	return socket.send(address, replyBuffer.data,replyBuffer.index);
}