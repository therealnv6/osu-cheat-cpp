#pragma once
#include <iostream>
#include <map>

class Signature
{
	std::string pattern;
	int offset;

	public:
		Signature()
		{
			this->pattern = "";
			this->offset = -1;
		}

		Signature(std::string pattern, int offset)
		{
			this->pattern = "";
			this->offset = -1;
		}

		std::string getPattern()
		{
			return this->pattern;
		}

		int getOffset()
		{
			return this->offset;
		}
};

Signature* TIME_SIGNATURE = new Signature("7E 55 8B 76 10 DB 05", 0x7);
Signature* MODE_SIGNATURE = new Signature("8D 45 BC 89 46 0C 83 3D", 0x8);
Signature* PLAYER_SIGNATURE = new Signature("FF 50 0C 8B D8 8B 15", 0x7); //todo: use this for beatmap recognition 