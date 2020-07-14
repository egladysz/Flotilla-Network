#include "FlotillaNet.h"

bool socketSetup()
{
	WSADATA wsa;
	return WSAStartup(MAKEWORD(2, 2), &wsa) == NO_ERROR;
}

bool socketShutDown()
{
	return WSACleanup();
}