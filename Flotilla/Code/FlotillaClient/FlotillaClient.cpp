#include "FlotillaClient.h"




FlotillaClient::FlotillaClient(Address server)
{
	serverAddress = server;
	clientSocket.open(0);

}

FlotillaClient::~FlotillaClient()
{
}


bool FlotillaClient::send(const Packet * const packet) const
{
	static uint8_t replyBufferArray[1536];
	Buffer replyBuffer;
	replyBuffer.data = replyBufferArray;
	replyBuffer.size = sizeof(replyBufferArray);
	replyBuffer.index = 0;
	packet->Write(replyBuffer);
	return clientSocket.send(serverAddress, replyBuffer.data, replyBuffer.index);
}