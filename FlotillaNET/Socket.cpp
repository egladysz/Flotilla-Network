#include "Socket.h"
#include "FlotillaNet.h"
#include <cstdio>
Socket::Socket()
	:handle{ 0 }
{

}

Socket::~Socket()
{
	close();
}

bool Socket::open(unsigned short port)
{
	handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (handle <= 0)
	{
		printf("Socket Creation Failed\n");
		handle = 0;
		return false;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(handle, (const sockaddr*)&addr, sizeof(sockaddr_in)) < 0)
	{
		printf("Socket Bind Failed\n");
		close();
		return false;
	}

	DWORD nonBlocking = 1;
	if (ioctlsocket(handle, FIONBIO, &nonBlocking) != 0)
	{
		printf("Socket Unblock Mode Failed\n");
		close();
		return false;
	}
	return isOpen();
}
void Socket::close()
{
	if (isOpen())
	{
		closesocket(handle);
	}
	handle = 0;
}
bool Socket::isOpen() const
{
	return handle > 0;
}
bool Socket::send(const Address & target, const void * buffer, int bufferSize) const
{
	sockaddr_in targetAddr;
	targetAddr.sin_family = AF_INET;
	targetAddr.sin_addr.s_addr = htonl(target.getAddress());
	targetAddr.sin_port = htons(target.getPort());
	int bytes = sendto(handle, (const char*)buffer, bufferSize, 0, (sockaddr*)&targetAddr, sizeof(sockaddr_in));

	if (bytes != bufferSize)
	{
		printf("Packet Send Failed\n");
		printf("%d\n", bytes);
		return false;
	}
	return true;
}
int Socket::receive(Address & sender, void * buffer, unsigned int bufferSize) const
{
	sockaddr_in senderAddr;
	int senderSize = sizeof(senderAddr);
	int bytes = recvfrom(handle, (char*)buffer, bufferSize, 0, (sockaddr*)&senderAddr, &senderSize);

	if (bytes <= 0)
	{
		//no data;
		return 0;
	}
	sender = Address(ntohl(senderAddr.sin_addr.s_addr), ntohs(senderAddr.sin_port));
	return bytes;
}