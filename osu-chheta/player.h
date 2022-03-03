#pragma once

#include <iostream>
#include "vector"
#include "mods.h"

class Beatmap {

};

class Player
{
	bool playing;
	Beatmap *currentMap;
	std::vector<Mod> currentMods;

	public:
		Player();

		virtual Beatmap getCurrentMap()
		{
			return *(this->currentMap);
		}

		void setCurrentMap(Beatmap map)
		{
			this->currentMap = &map;
		}

		virtual std::vector<Mod> getCurrentMods()
		{
			return this->currentMods;
		}

		void setCurrentMods(std::vector<Mod> mods)
		{
			this->currentMods = mods;
		}

		virtual bool isPlaying()
		{
			return this->playing && this->currentMap != nullptr;
		}

		void setPlaying(bool playing)
		{
			this->playing = playing;
		}
};