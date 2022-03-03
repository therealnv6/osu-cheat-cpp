#include "relax.hpp"
#include "signature.h"

//const char* sig = TIME_SIGNATURE->getPattern().c_str();
//const char* mask = "xxxxxxx";

template <typename T> T readMemory(char* section, HANDLE process)
{
    T value;

    ReadProcessMemory(process, section, &value, sizeof(T), NULL);
    return value;
}

void startRelax(Beatmap map, HANDLE process)
{
    printf("\nOverall Difficulty: %f", map.getOverallDifficulty());
    printf("\nApproach Rate: %f", map.getApproachRate());
    printf("\nCircle Size: %f", map.getCircleSize());
    printf("\nSlider Multiplier: %f\n", map.getSliderMultiplier());

    std::vector<Action> actions = map.getActions();

    auto action = actions.begin();
    int time = -1;

    char sig[7] = { 0x7E, 0x55, 0x8B, 0x76, 0x10, 0xDB, 0x05, };
    char mask[7] = { 'x', 'x', 'x', 'x', 'x', 'x', 'x', };

    //uint32_t timeSignature = (uint32_t)scanExtern(sig, mask, process);
    //uint32_t timeAddress = readMemory<uint32_t>((char*)timeSignature + 0x07, process);

    u8* timeAddress = ScanMemory(process, (u8*)sig, 0x07);

    while (!GetAsyncKeyState(VK_RETURN)) 
    {
        Sleep(100);
    }

    //std::cout << std::hex << "Time address: " << timeAddress << std::endl;

    while (action != actions.end())
    {
        time = readMemory<int>((char*)timeAddress, process);

        std::cout << time << " " << timeAddress << std::endl;

        if (time >= action->getTime() + 4)
        {
            INPUT input = action->getInput();

            SendInput(1, &input, sizeof(INPUT));
            action++;
        }
    }
}