#include <iostream>
#include <vector>

#include "relax.hpp"

const wchar_t* id = L"osu!.exe";

int main(int argc, char** argv)
{/*
    if (argc < 2) {
        std::cout << "no beatmap supplied" << std::endl;
        return 0;
	}*/

    HANDLE process = getProcess(id);

    if (process != nullptr)
    {
        std::cout << "Found osu process." << std::endl;

        std::string map = "C:/Users/Patrick/AppData/Local/osu!/Songs/916290 GALNEYRUS - RAISE MY SWORD (TV Size)/GALNEYRUS - RAISE MY SWORD (TV Size) (Orofil) [FOUR DIMENSIONS].osu";
        Beatmap beatmap = parseMap(map);

        startRelax(beatmap, process);
    }

    return 0;
}