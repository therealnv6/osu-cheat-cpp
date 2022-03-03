#include <iostream>
#include <vector>

#include "relax.hpp"

//const wchar_t* id = L"Dolphin.exe";

int main(int argc, char** argv)
{/*
    if (argc < 2) {
        std::cout << "no beatmap supplied" << std::endl;
        return 0;
	}*/

    HANDLE process = INVALID_HANDLE_VALUE;
    process = GetProcessHandle(L"Osu!.exe");

    if (process != INVALID_HANDLE_VALUE)
    {
        std::cout << "Found osu process: " << process << " / " << INVALID_HANDLE_VALUE << std::endl;

        std::string map = "C:/Users/Patrick/AppData/Local/osu!/Songs/1341551 Memme - Plasma Gun/Memme - Plasma Gun (emu1337) [Extra].osu";
        Beatmap beatmap = parseMap(map);

        startRelax(beatmap, process);
    }

    return 0;
}