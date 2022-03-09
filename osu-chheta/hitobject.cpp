#include "hitobject.hpp"

HitObject* createHitObject(std::string parse)
{
    auto vec = parseHitObject(parse);

    int x = std::stoi(vec[0]);
    int y = std::stoi(vec[1]);

    int time = std::stoi(vec[2]);
    int type = std::stoi(vec[3]);
    
    if (type & 0b1000)
    {
        return new SpinnerHitObject(x, y, time, std::stoi(vec[5]) - time);
    } else if(type & 0b0010)
    {
        return new SliderHitObject(x, y, time, std::stoi(vec[6]), std::stod(vec[7]));
    } else
    {
        return new CircleHitObject(x, y, time);
    }
}