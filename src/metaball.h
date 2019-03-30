#pragma once
#include "common.h"
#include <glm/glm.hpp>

struct Metaball
{
public:
    //  Every vec4 is encoded as [vec3(position), float(radius)]
    float metaballs[METABALL_COUNT][4];
    unsigned int metaballPosTexture;
public:
    Metaball();
    ~Metaball();
};
