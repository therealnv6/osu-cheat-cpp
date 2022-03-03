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

void sendInput(char key, boolean up)
{
    INPUT input;

    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    input.ki.wVk = VkKeyScan(key);
    input.ki.dwFlags = up;

    SendInput(1, &input, sizeof(INPUT));

}

void startRelax(Beatmap map, HANDLE process)
{
    printf("\nOverall Difficulty: %f", map.getOverallDifficulty());
    printf("\nApproach Rate: %f", map.getApproachRate());
    printf("\nCircle Size: %f", map.getCircleSize());
    printf("\nSlider Multiplier: %f\n", map.getSliderMultiplier());

    std::vector<Action> actions = map.getActions();

    auto action = actions.begin();
    auto end = actions[actions.size() - 1];

    char sig[7] = { 0x7E, 0x55, 0x8B, 0x76, 0x10, 0xDB, 0x05, };
    char mask[7] = { 'x', 'x', 'x', 'x', 'x', 'x', 'x', };

    u8* timeAddressAddress = ScanMemory(process, (u8*)sig, 0x07);

    char buffer[0x10];
    size_t bytesRead = 0;

    ReadProcessMemory(process, timeAddressAddress + 0x07, buffer, sizeof(u32), &bytesRead);

    u8* timeAddress = (u8*)(*(u32*)buffer);

    while (!GetAsyncKeyState(VK_RETURN)) 
    {
        Sleep(100);
    }

    while (action != actions.end())
    {
        ReadProcessMemory(process, timeAddress, buffer, sizeof(u32), &bytesRead);

        u32 currentTime = *(u32*)buffer;

        if (currentTime < end.getTime())
        {
            if (currentTime >= action->getTime())
            {
                INPUT input = action->getInput();

                SendInput(1, &input, sizeof(INPUT));

                printf("%i", action->getDuration());

                action++;
            }
        }
    }
}