#pragma once
#include "common.h"

struct PotentialField
{
public:
    unsigned int VBO;
    unsigned int VAO;
    
    glm::vec3 minCorner;
    glm::vec3 maxCorner;
    unsigned int LOD;   //  Number of voxel in each edge
    
    float* fieldData;
    size_t fieldSize;
    unsigned int voxelCount;
    float voxelHalfLength;
    float isoLevel;
public:
    /**
     *  Constructor
     */
    PotentialField() :
        minCorner(glm::vec3(-1, -1, -1)),
        maxCorner(glm::vec3(1, 1, 1)),
        LOD(10),
        fieldData(nullptr),
        fieldSize(0),
        voxelCount(1000),
        isoLevel(METABALL_COUNT)
    {
        ConstructVertexBuffer();
    }
    
    /**
     *  Constructor with user configuration
     *  @param min   - The minimum point for defining the bounding box
     *  @param max   - The maximum point for defining the bounding box
     *  @param count - Number of voxels along one axis
     */
    PotentialField(glm::vec3 min, glm::vec3 max, unsigned int count) :
        minCorner(min),
        maxCorner(max),
        LOD(count),
        fieldData(nullptr),
        fieldSize(0),
        voxelCount(count * count * count),
        isoLevel(METABALL_COUNT)
    {
        //  Check if the input value is invalid
        if(min.x - max.x == min.y - max.y == min.z - max.z)
        {
            std::cout << "Potential field must be a cube with equal side length!" << std::endl;
            return;
        }

        ConstructVertexBuffer();
    }
    
    /**
     *  Destructor
     */
    ~PotentialField()
    {
        //  Clean up work
        if(fieldData)
        {
            delete[] fieldData;
        }
        fieldData = nullptr;
    }
    
    /**
     *  Wrapper function for vertex buffer creation
     */
    void ConstructVertexBuffer()
    {
        //  Initialize scalar field
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
        
        //  Create VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, fieldSize, fieldData, GL_STATIC_DRAW);
        
        //  Create VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, NULL);
    }
};
