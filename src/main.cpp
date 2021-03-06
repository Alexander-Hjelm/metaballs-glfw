#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "metaball.h"
#include "shader.h"

PotentialField field(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 50);
const unsigned int metaballCount = 40;
const float speed = 0.1f;

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        field.isoLevel += speed;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        field.isoLevel -= speed;
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        field.GenerateRandomBalls(metaballCount, 1.0);
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

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
    
    // Shader initialization (should be created in run-time, because glad needs to be init first)
    Shader* shader = new Shader();
    glUseProgram(shader->Program);
    MarchingCube* marchingCube = new MarchingCube();

    // Vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, field.fieldSize, field.fieldData, GL_STATIC_DRAW);
    
    // Vertex array object
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, NULL);

    // Get an id for the metaball configuration
    unsigned int VHL = glGetUniformLocation(shader->Program, "voxelHalfLength");
    unsigned int MBC = glGetUniformLocation(shader->Program, "metaballCount");
    unsigned int IL = glGetUniformLocation(shader->Program, "isolevel");
    unsigned int VV = glGetUniformLocation(shader->Program, "viewVector");
    unsigned int triTex = glGetUniformLocation(shader->Program, "triTexture");
    unsigned int mbPosTex = glGetUniformLocation(shader->Program, "metaballPosTexture");
    unsigned int MVP_ID = glGetUniformLocation(shader->Program, "MVP");

    glUniform1i(MBC, metaballCount);
    glUniform3f(VV, -4,-3,-3);

    // Build metaballs array (randomly for now)
    field.GenerateRandomBalls(metaballCount, 1.0);
    float metaballTextureArray[metaballCount][4];


    unsigned int mbPosTexID;
    glGenTextures(1, &mbPosTexID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mbPosTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, metaballCount, 0, GL_RGBA, GL_FLOAT, &metaballTextureArray);
    
    //  Bind slot
    glUniform1i(triTex, 0);
    glUniform1i(mbPosTex, 1);
    
    glEnable(GL_DEPTH_TEST);
    float lastTime = glfwGetTime();
    float elapsedTime = 0.0f;
    float totalTime = 0.0f;
    float timer = 0.0f;
    int frame = 0;

    // render loop
    while(!glfwWindowShouldClose(window))
    {
        float deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        elapsedTime += deltaTime;
        totalTime += deltaTime;
        timer += deltaTime;
        
        //  One second has passed, log the fps on window title
        if(timer >= 1.0f)
        {
            std::string newTitle = "FPS: " + std::to_string(frame);
            std::cout << "FPS: " + std::to_string(frame) << std::endl;
            glfwSetWindowTitle(window, newTitle.c_str());
            timer -= 1.0f;
            frame = 0;
        }
        
        if(elapsedTime >= 0.016667f)      //  Target FPS is 60 fps
        {
            // input
            processInput(window);
       
            // rendering commands here
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // animate metaballs and send new texture to the shader
            field.Animate(elapsedTime);
            field.BuildTextureArray(metaballTextureArray);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mbPosTexID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, metaballCount, 0, GL_RGBA, GL_FLOAT, &metaballTextureArray);
    
            // Matrices
            const float FOV = 50.0f;
            bool rotate = false;
            glm::mat4 modelMatrix = glm::rotate((rotate)?totalTime:0.0f, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(-0.5, -0.5, -0.5));
            glm::mat4 viewMatrix = glm::lookAt(
                glm::vec3(1,1,1),   // Camera position
                glm::vec3(0,0,0),   // LookAt position
                glm::vec3(0,1,0)    // Up vector
            );
            glm::mat4 projectionMatrix = glm::perspective(
                glm::radians(FOV),  // FOV
                4.0f / 3.0f,        // Aspect ratio
                0.1f,               // Near clipping plane
                100.0f              // Far clipping plane
            );
            glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

            // Link mvp matrix with the currently boud GLSL shader
            glUniformMatrix4fv(MVP_ID, 1, false, &mvpMatrix[0][0]);
            glUniform1f(IL, field.isoLevel);
            glUniform1f(VHL, field.voxelHalfLength);
            glUniform3f(VV, -1,-1,-1);

            //  Draw
            glDrawArrays(GL_POINTS, 0, field.voxelCount);
            
            // check and call events and swap the buffers
            glfwPollEvents();
            glfwSwapBuffers(window);
            elapsedTime = 0.0;
            frame++;
        }
    }

    return 0;
}
