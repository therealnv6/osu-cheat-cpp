#include <iostream>
#include <iomanip>

#include "Utils.h"

void Utils::Hexdump(void* buffer, u32 length)
{
	u8* data = (u8*)buffer;

	std::cout << std::hex << std::setfill('0');

	for (u32 offset = 0; offset < length; offset += 16)
	{
		std::cout << std::setw(8) << offset;

		for (int i = 0; i < 16; i++)
		{
			if ((i + offset) >= length)
				std::cout << "   ";
			else
				std::cout << " " << std::setw(2) << (u32)(u8)data[offset + i];
		}

		std::cout << "  ";

		for (u32 i = 0; i < 16; i++)
		{
			if ((i + offset) >= length)
				std::cout << "   ";
			else
				std::cout << ascii(data[offset + i]);
		}

		std::cout << std::endl;
	}
}

char Utils::ascii(char c)
{
	if (c < 0x20 || c > 0x7E)
		return '.';

	return c;
}



template <typename T>
void Utils::Pack(bool pack, T* value)
{
	T temp;

	if (pack)
		temp = htonl(*value);
	else
		temp = ntohl(*value);

	*value = temp;
}