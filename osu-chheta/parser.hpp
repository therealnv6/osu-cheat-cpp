#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <Windows.h>
#include <functional>
#include "timingpoint.hpp"

#define PRIMARY_KEY 'z'
#define SECONDARY_KEY 'x'

class HitObject;
class Beatmap;

class Action
{
    int time;
    int duration;
    char key;
    INPUT input;
    bool pressed;

    public:
        Action();
        Action(HitObject* hitObject, char key, bool pressed);

        int getTime()
        {
            return this->time;
        }

        void setTime(int time)
        {
            this->time = time;
        }

        int getDuration()
        {
            return this->duration;
        }

        void setDuration(int duration)
        {
            this->duration = duration;
        }

        char getKey()
        {
            return this->key;
        }

        bool isPressed()
        {
            return this->pressed;
        }

        INPUT getInput()
        {
            return this->input;
        }

        void setInput(bool pressed);
};


std::vector<std::string> parseHitObject(std::string objectString);
Beatmap parseMap(std::string path); 
