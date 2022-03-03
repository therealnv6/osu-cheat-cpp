#include "timingpoint.hpp"

TimingPoint::TimingPoint()
{
    this->time = -1;
    this->beatLength = -1;
    this->inherited = false;
}

TimingPoint::TimingPoint(std::string parse)
{
    std::vector<std::string> vec = parseHitObject(parse);

    this->time = std::stoi(vec[0]);
    this->beatLength = std::stod(vec[1]);
    this->inherited = std::stoi(vec[6]);
}