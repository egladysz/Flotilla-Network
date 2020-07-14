#pragma once
#include <cstdint>
#include "PacketUtility.h"
constexpr uint16_t protocolID = 0xF10A;
namespace CommandType
{
	enum CommandType : uint8_t
	{
		ERR = 0,
		ConnectRequest,
		ConnectReply,
		Movement,
		MoveACK,
		Radio,
		RadioACK,
		Disconnect,
		DisconnectACK
	};
}
struct Packet
{
	uint16_t protocol = protocolID;
	CommandType::CommandType command;
	virtual void Read(Buffer& buffer);
	virtual void Write(Buffer& buffer) const;
};


struct ConnectRequest : Packet
{
	ConnectRequest() {command = CommandType::ConnectRequest;}
};

struct ConnectReply : Packet
{
	ConnectReply() { command = CommandType::ConnectReply; }
	bool accepted;
	void Read(Buffer& buffer)
	{
		Packet::Read(buffer);
		accepted = ReadByte(buffer);
	}
	void Write(Buffer& buffer) const
	{
		Packet::Write(buffer);
		WriteByte(buffer, accepted);
	}
};

struct Movement : Packet
{
	Movement() { command = CommandType::Movement; }
	enum Direction:uint8_t{ NONE, UP, DOWN , LEFT, RIGHT, UL, UR, DL, DR};
	int sequence;
	Direction direction;
	float x;
	float y;
	void Read(Buffer& buffer)
	{
		Packet::Read(buffer);
		sequence = ReadInteger(buffer);
		direction = static_cast<Direction>(ReadByte(buffer));
		x = ReadFloat(buffer);
		y = ReadFloat(buffer);
	}
	void Write(Buffer& buffer) const
	{
		Packet::Write(buffer);
		WriteInteger(buffer, sequence);
		WriteByte(buffer, direction);
		WriteFloat(buffer, x);
		WriteFloat(buffer, y);
	}
};

struct MoveACK : Movement
{
	MoveACK() { command = CommandType::MoveACK; }

};

constexpr unsigned int MaxRadioPerPacket = 255;
struct RadioPoint
{
	float x;
	float y;
	float hit;
};

struct Radio : Packet
{
	Radio() { command = CommandType::Radio; }

	uint32_t sequence;
	uint8_t count;
	RadioPoint points[MaxRadioPerPacket];
	void Read(Buffer& buffer)
	{
		Packet::Read(buffer);
		sequence = ReadInteger(buffer);
		count = ReadByte(buffer);
		for (int i = 0; i < count; ++i)
		{
			points[i].x = ReadFloat(buffer);
			points[i].y = ReadFloat(buffer);
			points[i].hit = ReadFloat(buffer);
		}
	}
	void Write(Buffer& buffer) const
	{
		Packet::Write(buffer);
		WriteInteger(buffer, sequence);
		WriteByte(buffer, count);
		for (int i = 0; i < count; ++i)
		{
			WriteFloat(buffer, points[i].x);
			WriteFloat(buffer, points[i].y);
			WriteFloat(buffer, points[i].hit);
		}
	}
};

struct RadioACK : Packet
{
	RadioACK() { command = CommandType::RadioACK; }

	int sequence;
	unsigned char count;
	void Read(Buffer& buffer)
	{
		Packet::Read(buffer);
		sequence = ReadInteger(buffer);
		count = ReadByte(buffer);
	}
	void Write(Buffer& buffer) const
	{
		Packet::Write(buffer);
		WriteInteger(buffer, sequence);
		WriteByte(buffer, count);
	}
};

struct Disconnect : Packet
{
	Disconnect() { command = CommandType::Disconnect; }

};

struct DisconnectACK : Packet
{
	DisconnectACK() { command = CommandType::DisconnectACK; }

};