#include "Address.h"
Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port)
{
	unsigned int add = (a << 24) | (b << 16) | (c << 8) | d;
	setup(add, port);
}

Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	unsigned int add = (a << 24) | (b << 16) | (c << 8) | d;
	setup(add, 0);
}

Address::Address(unsigned int hostaddr, unsigned short port)
{
	setup(hostaddr, port);
}

Address::Address(unsigned int hostaddr)
{
	setup(hostaddr, 0);
}

Address::Address(unsigned short port)
{
	setup(0, port);
}

Address::Address()
{
	setup(0, 0);
}

unsigned int Address::getAddress() const
{
	return address;
}

unsigned short Address::getPort() const
{
	return port;
}

bool Address::operator==(const Address & other) const
{
	return (address == other.address) && (port == other.port);
}

bool Address::operator!=(const Address & other) const
{
	return !(*this == other);
}

void Address::setup(unsigned int hAddr, unsigned short hPort)
{
	address = hAddr;
	port = hPort;
}
