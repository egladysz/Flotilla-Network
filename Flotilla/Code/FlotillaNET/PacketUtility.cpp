#include "PacketUtility.h"
#include "FlotillaNet.h"

union floatCast {
	float f;
	uint32_t i;
};
void WriteFloat(Buffer & buffer, float value)
{

	floatCast ert;
	ert.f = value;
	WriteInteger(buffer, ert.i);
}
void WriteInteger(Buffer & buffer, uint32_t value)
{
	if (buffer.index + 4 > buffer.size) return;
	*((uint32_t*)(buffer.data + buffer.index)) = htonl(value);
	buffer.index += 4;
}

void WriteShort(Buffer & buffer, uint16_t value)
{
	if (buffer.index + 2 > buffer.size) return;
	*((uint16_t*)(buffer.data + buffer.index)) = htons(value);
	buffer.index += 2;
}

void WriteByte(Buffer & buffer, uint8_t value)
{
	if (buffer.index + 1 > buffer.size) return;
	*((uint8_t*)(buffer.data + buffer.index)) = value;
	buffer.index += 1;
}

float ReadFloat(Buffer & buffer)
{
	floatCast ert;
	ert.i = ReadInteger(buffer);
	return ert.f;
}

uint32_t ReadInteger(Buffer & buffer)
{
	if (buffer.index + 4 > buffer.size) return 0;
	uint32_t value;
	value = value = ntohl(*((uint32_t*)(buffer.data + buffer.index)));
	buffer.index += 4;
	return value;
}

uint16_t ReadShort(Buffer & buffer)
{
	if (buffer.index + 2 > buffer.size) return 0;
	uint16_t value;
	value = value = ntohs(*((uint16_t*)(buffer.data + buffer.index)));
	buffer.index += 2;
	return value;
}

uint8_t ReadByte(Buffer & buffer)
{
	if (buffer.index + 1 > buffer.size) return 0;
	uint8_t value;
	value = value = *((uint8_t*)(buffer.data + buffer.index));
	buffer.index += 1;
	return value;
}