#include "offset.h"
#include "hitobject.hpp"
#include "beatmap.h"

const double AR_10_OFFSET = 2.7;
const double TIME_DIFFERENCE_MULTIPLIER = 0.94;

int previousOffset = 0;

int calculateOffset(HitObject* object, std::vector<HitObject*> objects, Beatmap beatmap)
{
    HitObject* nextObject = nullptr;
    bool isNext = false;

    for (auto current : objects)
    {
        if (isNext)
        {
            nextObject = current;
            break;
        }

        if (object == current)
        {
            isNext = true;
        }
    }

    if (nextObject == nullptr)
    {
        // object does not have a next object, offset should just be random (probably.)
        return 0; // todo: make random offset
    }

    int offsetRangeMaximum = 0;

    int timeDifference = object->getTime() - nextObject->getTime();
    int approachRate = beatmap.getApproachRate();

    if (timeDifference <= 150)
    {
        offsetRangeMaximum += timeDifference *= TIME_DIFFERENCE_MULTIPLIER;


        if (approachRate >= 10)
        {
            offsetRangeMaximum *= (AR_10_OFFSET * approachRate);
        }
    }

    if (offsetRangeMaximum == 0)
    {
        return (rand() % 200);
    }

    int offset = rand() % offsetRangeMaximum;

    return (offset);
}