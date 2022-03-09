#include "parser.hpp"
#include "hitobject.hpp"
#include "beatmap.h"
#include "offset.h"

Action::Action()
{
    this->time = -1;
    this->duration = -1;
    this->key = '\0';
    this->input = {};

    this->pressed = false;
}

Action::Action(HitObject* hitObject, char key, bool pressed, int offset)
{
    this->time = hitObject->getTime();
    this->duration = -1;

    if (hitObject->getType() == HitObjectType::Spinner)
    {
        this->duration = dynamic_cast<SpinnerHitObject*>(hitObject)->getEndTime() - hitObject->getTime();
    }

    if(!pressed)
    {
        if (hitObject->getType() == HitObjectType::Circle)
        {
            this->time += offset;
        } else
        {
            this->time += this->duration;
        }
    }

    this->key = key;
    this->setInput(pressed);
}

Action::Action(double sliderMultiplier, double beatLength, TimingPoint* timingPoint, HitObject* hitObject, char key, bool pressed, int offset)
{
    SliderHitObject* sliderObject = dynamic_cast<SliderHitObject*>(hitObject);

    this->time = hitObject->getTime();
    this->duration = (sliderObject->getSliderLength() / (sliderMultiplier * 100) * (beatLength * (timingPoint->isInherited() ? 1.0 : std::abs(timingPoint->getBeatLength()) / 100))) * sliderObject->getSlides();

    if (!pressed)
    {
        this->time += this->duration;
    }

    this->key = key;
    this->setInput(pressed);
}

void Action::setInput(bool pressed)
{
    this->pressed = pressed;
    this->input = { };

    this->input.type = INPUT_KEYBOARD;
    this->input.ki.wVk = VkKeyScan(key);
    this->input.ki.dwFlags = (pressed ? 0 : KEYEVENTF_KEYUP);
}

std::vector<std::string> parseHitObject(std::string objectString)
{
    std::vector<std::string> vec;

    int delimeter = 0;

    while ((delimeter = objectString.find(",")) != std::string::npos)
    {
        vec.push_back(objectString.substr(0, delimeter));
        objectString.erase(0, delimeter + 1);
    }

    if (!objectString.empty())
    {
        vec.push_back(objectString);
    }

    return vec;
}

const std::map<std::string, int> sectionMap = {
    { "[TimingPoints]", 0 },
    { "[HitObjects]", 1 },
    { "[Difficulty]", 2 }
};

using funcPtr = void(Beatmap::*)(float);

std::map<std::string, funcPtr> beatmapInitializers = {
    { "ApproachRate:", &Beatmap::setApproachRate },
    { "OverallDifficulty:", &Beatmap::setOverallDifficulty },
    { "CircleSize:", &Beatmap::setCircleSize },
    { "SliderMultiplier:", &Beatmap::setSliderMultiplier },
};

double getCorrectBeatLength(std::vector<TimingPoint>* timingPoints, std::vector<TimingPoint>::iterator currentTimingPoint, std::vector<HitObject*> hitObjects)
{
    double beatLength = currentTimingPoint->getBeatLength();

    while (currentTimingPoint != timingPoints->end() - 1 && currentTimingPoint->getTime() != hitObjects[0]->getTime())
    {
        currentTimingPoint++;

        if (currentTimingPoint->isInherited())
        {
            beatLength = currentTimingPoint->getBeatLength();
        }
    }

    return beatLength;
}

Beatmap parseMap(std::string path)
{
    Beatmap* beatmap = new Beatmap();

    std::ifstream file(path);
    std::vector<Action> actions;

    if (!file)
    {
        return *beatmap;
    }

    std::vector<HitObject*> hitObjects;
    std::vector<TimingPoint> timingPoints;

    std::string line;

    int section = -1;

    std::cout << "[parsing] parsing contents of" + path + "osu! beatmap" << std::endl;

    while (std::getline(file, line))
    {
        if (!strcmp(line.c_str(), ""))
        {
            section = -1;
        } else {
            bool shouldContinue = true;

            for (auto entry : sectionMap)
            {
                if (entry.first.compare(line) == 0)
                {
                    section = entry.second;
                    shouldContinue = false;
                    break;
                }
            }

            if (shouldContinue)
            {
                if (section == 0)
                {
                    timingPoints.push_back(TimingPoint(line));
                }
                else if (section == 1)
                {
                    hitObjects.push_back(createHitObject(line));
                }
                else if (section == 2)
                {
                    for (auto initializer : beatmapInitializers)
                    {
                        auto function = initializer.second;

                        if (line.find(initializer.first) == 0)
                        {
                            (beatmap->*function)(std::stod(line.substr(line.find(":") + 1)));
                        }

                        if (beatmap->isFilled())
                        {
                            section = -1;
                        }
                    }
                }
            }
        }
    }

    auto currentTimingPoint = timingPoints.begin();

    double beatLength = timingPoints.begin()->getBeatLength();
    char key = PRIMARY_KEY;

    int lastTime = 0;

    printf("[actions] creating actions from HitObjects\n");

    for (auto object : hitObjects)
    {
        beatLength = getCorrectBeatLength(&timingPoints, currentTimingPoint, hitObjects);

        if (currentTimingPoint->isInherited())
        {
            beatLength = currentTimingPoint->getBeatLength();
        }

        int offset = calculateOffset(object, hitObjects, *beatmap);
        std::cout << offset << std::endl;

        if (object->getType() == HitObjectType::Circle || object->getType() == HitObjectType::Spinner)
        {
            actions.push_back(Action(object, key, true, offset));
            actions.push_back(Action(object, key, false, offset));
        }
        else if (object->getType() == HitObjectType::Slider)
        {
            actions.push_back(Action(beatmap->getSliderMultiplier(), beatLength, &*currentTimingPoint, object, key, true, offset));
            actions.push_back(Action(beatmap->getSliderMultiplier(), beatLength, &*currentTimingPoint, object, key, false, offset));
        }

        if (object->getTime() - lastTime <= 10 || object->getType() == HitObjectType::Slider)
        {
            if (key == PRIMARY_KEY)
            {
                key = SECONDARY_KEY;
            }
            else
            {
                key = PRIMARY_KEY;
            }
        }
        else
        {
            key = PRIMARY_KEY;
        }

        lastTime = object->getTime();
    }

    beatmap->setActions(actions);

    return *beatmap;
}