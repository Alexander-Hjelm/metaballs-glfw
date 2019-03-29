#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
struct Metaball
{
public:
    glm::vec3 position;
    float radius;
public:
    Metaball() :
        position(glm::vec3(0, 0, 0)),
        radius(1)
    {
    }
    Metaball(glm::vec3 pos, float r) :
        position(pos),
        radius(r)
    {
    }
};
