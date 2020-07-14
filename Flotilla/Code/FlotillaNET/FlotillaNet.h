#pragma once
#include <winsock2.h>
#pragma comment(lib, "wsock32.lib")

#include "Socket.h"
bool socketSetup();
bool socketShutDown();
