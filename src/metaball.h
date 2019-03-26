#pragma once

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
    inline void Print() { std::cout << "Position: " << glm::to_string(position) << "\tRadius: " << radius << std::endl; }
};

struct PotentialField
{
public:
    glm::vec3 minCorner;
    glm::vec3 maxCorner;
    unsigned int LOD;   //  Number of voxel in each edge
    
    float* fieldData;
    size_t fieldSize;
    unsigned int voxelCount;
    float voxelHalfLength;
public:
    PotentialField() :
        minCorner(glm::vec3(-1, -1, -1)),
        maxCorner(glm::vec3(1, 1, 1)),
        LOD(10),
        fieldData(nullptr),
        fieldSize(0),
        voxelCount(1000)
    {
        ConstructVertexBuffer();
    }
    PotentialField(glm::vec3 min, glm::vec3 max, unsigned int count) :
        minCorner(min),
        maxCorner(max),
        LOD(count),
        fieldData(nullptr),
        fieldSize(0),
        voxelCount(count * count * count)
    {
        if(min.x - max.x == min.y - max.y == min.z - max.z)
        {
            std::cout << "Potential field must be a cube with equal side length!" << std::endl;
            return;
        }
        ConstructVertexBuffer();
    }
    ~PotentialField()
    {
        if(fieldData)
        {
            delete[] fieldData;
        }
        fieldData = nullptr;
    }
    void ConstructVertexBuffer()
    {
        fieldData = new float[voxelCount * 3];
        fieldSize = voxelCount * 3 * sizeof(float);
        voxelHalfLength = (maxCorner.x - minCorner.x) / LOD / 2.0f;
        for(int i = 0; i < LOD; ++i)
        {
            for(int j = 0; j < LOD; ++j)
            {
                for(int k = 0; k < LOD; ++k)
                {
                    fieldData[(i*LOD*LOD + j*LOD + k) * 3 + 0] = minCorner.x + i * 2 * voxelHalfLength + voxelHalfLength;
                    fieldData[(i*LOD*LOD + j*LOD + k) * 3 + 1] = minCorner.y + j * 2 * voxelHalfLength + voxelHalfLength;
                    fieldData[(i*LOD*LOD + j*LOD + k) * 3 + 2] = minCorner.z + k * 2 * voxelHalfLength + voxelHalfLength;
                }
            }
        }
    }
    inline void Print() { std::cout << "Area: " << glm::to_string(minCorner) << " to " << glm::to_string(maxCorner) << ", with a total of " << voxelCount << " voxels." << std::endl; }
};
