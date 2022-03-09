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

        std::string map = "C:/Users/Patrick/AppData/Local/osu!/Songs/842412 Will Stetson - Harumachi Clover (Swing Arrangement) [Dictate Edit]/Will Stetson - Harumachi Clover (Swing Arrangement) [Dictate Edit] (Sotarks) [Oh no!].osu";
        Beatmap beatmap = parseMap(map);

        startRelax(beatmap, process);
    }

    return 0;
}