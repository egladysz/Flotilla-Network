#include "Packet.h"

void Packet::Read(Buffer& buffer)
{
	protocol = ReadShort(buffer);
	if (protocol != protocolID)
	{
		command = CommandType::ERR;
		return;
	}
	command = static_cast<CommandType::CommandType>(ReadByte(buffer));
}

void Packet::Write(Buffer& buffer) const
{
	WriteShort(buffer, protocol);
	WriteByte(buffer, command);
}
