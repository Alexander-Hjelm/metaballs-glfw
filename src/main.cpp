#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "metaball.h"
#include "potentialfield.h"
#include "marchingcube.h"
#include "shader.h"

PotentialField* field;

void processInput(GLFWwindow *window)
{
    float multiplier = 0.01f * METABALL_COUNT;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Adjust potential field threshold
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        field->isoLevel += multiplier;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        field->isoLevel -= multiplier;
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Metaball", NULL, NULL);

    glfwMakeContextCurrent( window );
    

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Enable z-buffer depth test
    glEnable(GL_DEPTH_TEST);
    
    // GLAD dependent class initialization
    Shader* shader = new Shader();
    MarchingCube* marchingCube = new MarchingCube();
    Metaball metaball;
    field = new PotentialField(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), 20);
    
    // Model matrx (identity, model will be at the origin)
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    //glm::mat4 viewMatrix = glm::translate(glm::mat4(), glm::vec3(-3.0f, 0.0f, 0.0f));
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(4,3,3),   // Camera position
        glm::vec3(0,0,0),   // LookAt position
        glm::vec3(0,1,0)    // Up vector
    );

    float FOV = 50.0f;
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(FOV),  // FOV
        4.0f / 3.0f,        // Aspect ratio
        0.1f,               // Near clipping plane
        100.0f              // Far clipping plane
    );

    glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    // Get the location for all uniforms and texture samplers
    unsigned int MVP_ID = glGetUniformLocation(shader->Program, "MVP");
    shader->SetUniform("voxelHalfLength", field->voxelHalfLength);
    shader->SetUniform("metaballPosTexture", 0u);
    shader->SetUniform("triTexture", 1u);
    
    float lastTime = 0.0f;
    float elapsedTime = 0.0f;
    // render loop
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Link mvp matrix with the currently boud GLSL shader
        glUniformMatrix4fv(MVP_ID, 1, false, &mvpMatrix[0][0]);

        // Update shader uniform
        shader->SetUniform("isolevel", field->isoLevel);

        // Draw all voxels in the potential field
        glDrawArrays(GL_POINTS, 0, field->voxelCount);
        
        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}
