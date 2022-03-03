#pragma once

#include <string>
#include <vector>
#include "parser.hpp"
  
enum class HitObjectType
{
    Circle,
    Slider,
    Spinner,
};

class HitObject
{
    protected:
        int x;
        int y;
        int time;

        HitObjectType type;

    public:
        HitObject()
        {
            this->x = -1;
            this->y = -1;
            this->time = -1;
            this->type = HitObjectType::Circle;
        }

        int getTime()
        {
            return time;
        }

        virtual HitObjectType getType()
        {
            return type;
        }
};

class CircleHitObject : public HitObject {
    public:
        CircleHitObject(int x, int y, int time)
        {
            this->x = x;
            this->y = y;
            this->time = time;
            this->type = HitObjectType::Circle;
        }
};

class SliderHitObject : public HitObject 
{
    int slides;
    double sliderLength;

    public:
        SliderHitObject(int x, int y, int time, int slides, double sliderLength)
        {
            this->x = x;
            this->y = y;
            this->time = time;

            this->type = HitObjectType::Slider;

            // slider-specific fields        
            this->slides = slides;
            this->sliderLength = sliderLength;
        }

        int getSlides()
        {
            return this->slides;
        }

        double getSliderLength()
        {
            return this->sliderLength;
        }
};

class SpinnerHitObject : public HitObject
{
    int endTime;

    public:
        SpinnerHitObject(int x, int y, int time, int endTime)
        {
            this->x = x;
            this->y = y;
            this->time = time;

            this->type = HitObjectType::Spinner;

            // spinner-specific fields
            this->endTime = endTime;
        }

        int getEndTime()
        {
            return this->endTime;
        }
};

HitObject createHitObject(std::string parse);