#pragma once
#include "../FlotillaNET/Server.h"
#include "ClientPlayer.h"
class FlotillaServer : public Server
{
private:
	
public:
	FlotillaServer(unsigned short port);
	ClientPlayer players[maxClientSlots];
};

