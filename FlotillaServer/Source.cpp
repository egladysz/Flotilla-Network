#include "FlotillaNet.h"
#include "FlotillaServer.h"
#include "PacketUtility.h"
#include "Packet.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <iterator>

#include <stdlib.h>
#include <time.h>

void processMessage(uint8_t* bufferArray, int size, Address sender, FlotillaServer& server);
void broadcastState(FlotillaServer& server);
int main()
{
	srand(time(NULL));
	socketSetup();
	FlotillaServer server(30000);

	uint8_t bufferArray[1536];
	

	bool running = true;
	Address sender;
	int sequence = 0;
	while (running)
	{
		broadcastState(server);
		
		int gottenBytes = server.socket.receive(sender, bufferArray, sizeof(bufferArray));
		while(gottenBytes > 0)
		{
			processMessage(bufferArray, gottenBytes, sender, server);
			gottenBytes = server.socket.receive(sender, bufferArray, sizeof(bufferArray));
		}
		
		std::vector<std::pair<ClientPlayer, Address>> activeClients;
		for (int i = 0; i < maxClientSlots; i++)
		{
			if (server.isClientConnected(i))
			{
				activeClients.emplace_back(std::pair<ClientPlayer, Address>(server.players[i], server.getClientAddress(i)));
			}
			
		}

		for (const auto& p : activeClients)
		{
			auto player = p.first;
			auto addr = p.second;
			RadioPoint attemptHits[50];
			int hitCount = 0;
			for (int attempts = 0; attempts < 50; attempts++)
			{
				unsigned char attX = (rand() + 256) % 256;
				unsigned char attY = (rand() + 256) % 256;
				float attPosX = (player.x - 128) + attX;
				float attPosY = (player.y - 128) + attY;

				for (const auto& playerOb : activeClients)
				{
					const auto& otherPlayer = playerOb.first;
					if (/*playerOb.second != addr && */otherPlayer.isColliding(attPosX, attPosY))
					{
						RadioPoint hit;
						hit.hit = (otherPlayer.radius - sqrtf((attPosX - otherPlayer.x)*(attPosX - otherPlayer.x) + (attPosY - otherPlayer.y)*(attPosY - otherPlayer.y)));
						hit.x = attPosX;
						hit.y = attPosY;
						attemptHits[hitCount++] = hit;
						continue;
					}
				}
			}
			Radio radioResult;
			radioResult.count = hitCount;
			radioResult.sequence = sequence;
			std::copy(std::begin(attemptHits), std::begin(attemptHits) + hitCount, std::begin(radioResult.points));
			server.send(addr, &radioResult);
			sequence++;
		}
		Sleep(100/6);
	}
	socketShutDown();
	return 0;
}





void processMessage(uint8_t* bufferArray, int size, Address sender, FlotillaServer& server)
{
	Buffer reader;
	reader.data = bufferArray;
	reader.index = 0;
	reader.size = size;
	Packet packetHead;
	packetHead.Read(reader);
	int playerIndex = server.findClientSlotIndex(sender);
	if (packetHead.command == CommandType::ConnectRequest)
	{
		ConnectReply reply;

		if (playerIndex != -1)
		{
			// already here. ack and proceed
			// assuming mass connect attempt to ack approved
			reply.accepted = true;
		}
		else
		{
			std::cout << "Player Connection Detected" << std::endl;
			//not connected. add and ack on result
			int clientIndex = server.addClient(sender);
			if (clientIndex == -1)
			{
				//no space. failure
				reply.accepted = false;
			}
			else
			{
				server.players[clientIndex] = ClientPlayer{};
				server.players[clientIndex].x = 50 * clientIndex;
				reply.accepted = true;
				std::cout << "Player " << clientIndex << " has Connected!\n";
			}
		}
		server.send(sender,&reply);
	}
	else if (playerIndex == -1)
	{
		//not an accepted address. ignore
		return;
	}
	else if (packetHead.command == CommandType::Movement)
	{
		Movement movementPacket;
		reader.index = 0;
		movementPacket.Read(reader);
		float dx = 0;
		float dy = 0;
		switch (movementPacket.direction)
		{
		case Movement::UP:
		case Movement::UL:
		case Movement::UR:
			dy++;
			break;
		case Movement::DOWN:
		case Movement::DL:
		case Movement::DR:
			dy--;
			break;
		}
		switch (movementPacket.direction)
		{
		case Movement::LEFT:
		case Movement::UL:
		case Movement::DL:
			dx--;
			break;
		case Movement::RIGHT:
		case Movement::UR:
		case Movement::DR:
			dx++;
			break;
		}

		dx *= 0.75;
		dy *= 0.75;

		server.players[playerIndex].x += dx;
		server.players[playerIndex].y += dy;

		MoveACK reply;
		reply.direction = movementPacket.direction;
		reply.sequence = movementPacket.sequence;
		reply.x = server.players[playerIndex].x;
		reply.y = server.players[playerIndex].y;
		server.send(sender, &reply);

	}
	else if (packetHead.command == CommandType::Disconnect)
	{
		DisconnectACK reply;
		int clientIndex = server.findClientSlotIndex(sender);
		server.removeClient(sender);
		
		std::cout << "Player " << clientIndex << " has Disconnected!" << std::endl;

		server.send(sender, &reply);

	}
}

void broadcastState(FlotillaServer & server)
{
}
