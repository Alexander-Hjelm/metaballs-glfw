#include "metaball.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/**
 *  Constructor
 */
Metaball::Metaball()
{
    for(int i = 0; i < METABALL_COUNT; ++i)
    {
        metaballs[i][0] = float(rand()) / float(RAND_MAX) * 2 - 1;   // [-1, 1]
        metaballs[i][1] = float(rand()) / float(RAND_MAX) * 2 - 1;   // [-1, 1]
        metaballs[i][2] = float(rand()) / float(RAND_MAX) * 2 - 1;   // [-1, 1]
        metaballs[i][3] = float(rand()) / float(RAND_MAX);           // [ 0, 1]
    }
    
    glGenTextures(1, &metaballPosTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, metaballPosTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, METABALL_COUNT, 0, GL_RGBA, GL_FLOAT, metaballs);
}

/**
 *  Destructor
 */
Metaball::~Metaball()
{
}
