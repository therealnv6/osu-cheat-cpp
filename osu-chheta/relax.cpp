#include "relax.hpp"
#include "signature.h"

const char* sig = TIME_SIGNATURE->getPattern().c_str();
const char* mask = "xxxxxxx";

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

    uint32_t timeSignature = (uint32_t)scanExtern(const_cast<char*&>(sig), const_cast<char*&>(mask), &process);
    uint32_t timeAddress = readMemory<uint32_t>((char*)timeSignature + TIME_SIGNATURE->getOffset(), process);

    while (!GetAsyncKeyState(VK_RETURN)) {
        Sleep(100);
    }

    std::cout << "here" << std::endl;

    while (action != actions.end())
    {
        std::cout << "here 2" << std::endl;
        time = readMemory<int>((char*)timeAddress, process);

        std::cout << time + " " + timeAddress << std::endl;

        if (time >= action->getTime() + 4)
        {
            INPUT input = action->getInput();

            SendInput(1, &input, sizeof(INPUT));
            action++;
        }
    }
}