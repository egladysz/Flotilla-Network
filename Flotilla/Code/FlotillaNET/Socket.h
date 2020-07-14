#pragma once
#include "Address.h"

class Socket
{
public:
	Socket();
	~Socket();

	bool open(unsigned short port);

	void close();

	bool isOpen() const;

	bool send(const Address& target, const void* buffer, int bufferSize) const;
	int receive(Address& sender, void* buffer, unsigned int bufferSize) const;


private:
	int handle;

};