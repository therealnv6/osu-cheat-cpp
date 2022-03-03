#pragma once

#include <string>
#include <vector>

#include "parser.hpp"

class TimingPoint
{
    int time;
    double beatLength;
    bool inherited;

    public:
        TimingPoint();
        TimingPoint(std::string parse);

        int getTime()
        {
            return this->time;
        }

        double getBeatLength()
        {
            return this->beatLength;
        }

        bool isInherited()
        {
            return this->inherited;
        }
};