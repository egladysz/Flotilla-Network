#pragma once
class Address
{
public:
	Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port);
	Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
	Address(unsigned int hostaddr, unsigned short port);
	Address(unsigned int hostaddr);
	Address(unsigned short port);
	Address();
	unsigned int getAddress() const;
	unsigned short getPort() const;
	bool operator==(const Address& other) const;
	bool operator!=(const Address& other) const;

private:
	unsigned int address;
	unsigned short port;

	void setup(unsigned int hAddr, unsigned short hPort);
};