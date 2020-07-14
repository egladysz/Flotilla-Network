#pragma once
#include <cstdint>

struct Buffer
{
	uint8_t* data;
	int size;
	int index;
};

void WriteFloat(Buffer & buffer, float value);
void WriteInteger(Buffer & buffer, uint32_t value);
void WriteShort(Buffer & buffer, uint16_t value);
void WriteByte(Buffer & buffer, uint8_t value);

float ReadFloat(Buffer & buffer);
uint32_t ReadInteger(Buffer & buffer);
uint16_t ReadShort(Buffer & buffer);
uint8_t ReadByte(Buffer & buffer);