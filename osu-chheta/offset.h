#pragma once
#include <vector>

class HitObject;
class Beatmap;

int calculateOffset(HitObject* object, std::vector<HitObject*> objects, Beatmap beatmap);