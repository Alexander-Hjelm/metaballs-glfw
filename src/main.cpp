#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "metaball.h"
#include "shader.h"

Shader* shader;  //  Let's make the shader global at this moment, just for the sake of recompilation
MarchingCube* marchingCube;
PotentialField field(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), 80);
unsigned const int metaballCount = 10;

void processInput(GLFWwindow *window)
{
    float speed = 0.1f;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        shader->RecompileAndLink(); //  Recompile shader if "R" is pressed
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        field.isoLevel += speed;
//        ball.position.z += speed;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        field.isoLevel -= speed;
//        ball.position.z -= speed;
//    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        ball.position.x += speed;
//    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        ball.position.x -= speed;
//    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//        ball.position.y += speed;
//    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
//        ball.position.y -= speed;
//    {   //  metaball out-of-bound check, nothing to do with glfw input
//        if(ball.position.x < field.minCorner.x)
//            ball.position.x = field.maxCorner.x;
//        if(ball.position.x > field.maxCorner.x)
//            ball.position.x = field.minCorner.x;
//        if(ball.position.y < field.minCorner.y)
//            ball.position.y = field.maxCorner.y;
//        if(ball.position.y > field.maxCorner.y)
//            ball.position.y = field.minCorner.y;
//        if(ball.position.z < field.minCorner.z)
//            ball.position.z = field.maxCorner.z;
//        if(ball.position.z > field.maxCorner.z)
//            ball.position.z = field.minCorner.z;
//    }
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
    shader = new Shader();
    glUseProgram(shader->Program);
    marchingCube = new MarchingCube();

    // Vertex buffer object, gives instant access to vertices in the GPU
    // GenBuffers yields a unique ID for the newly created buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    // Specify type of the vertex buffer and bind it to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy the vertex data to the boud buffer's memory
    // GL_STATIC_DRAW specifies how the GPU should manage the data
    // (static = the data will likely not change)
    glBufferData(GL_ARRAY_BUFFER, field.fieldSize, field.fieldData, GL_STATIC_DRAW);

    // Vertex array object
    // Holds all our buffers
    // When we want to render all triangles we can simply bind the vertex array and draw
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Specify the attribute location (0) of the first vertex buffer, and the type of data.
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, NULL);

    // Model matrx (identity, model will be at the origin)
    glm::mat4 modelMatrix = glm::mat4(1.0f);

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

    // Get an id for the mvp matrixmvp matrix
    unsigned int MVP_ID = glGetUniformLocation(shader->Program, "MVP");
    // Get an id for the metaball configuration
    unsigned int VHL = glGetUniformLocation(shader->Program, "voxelHalfLength");
    unsigned int MBC = glGetUniformLocation(shader->Program, "metaballCount");
    unsigned int IL = glGetUniformLocation(shader->Program, "isolevel");
    unsigned int VV = glGetUniformLocation(shader->Program, "viewVector");
    unsigned int triTex = glGetUniformLocation(shader->Program, "triTexture");
    unsigned int mbPosTex = glGetUniformLocation(shader->Program, "metaballPosTexture");

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
    float timer = 0.0f;
    int frame = 0;

    // render loop
    while(!glfwWindowShouldClose(window))
    {
        float deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        elapsedTime += deltaTime;
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
        
        if(elapsedTime >= 0.013f)      //  Render at 60 fps
        {
            // input
            processInput(window);
       
            // rendering commands here
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // animate metaballs and send new texture to the shader
            field.Animate(elapsedTime);
            field.BuildTextureArray(metaballTextureArray);

            // TODO: not all these texture settings may be neccessary, try removing one line at a time
            glGenTextures(1, &mbPosTexID);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mbPosTexID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, metaballCount, 0, GL_RGBA, GL_FLOAT, &metaballTextureArray);
    
            // Link mvp matrix with the currently boud GLSL shader
            glUniformMatrix4fv(MVP_ID, 1, false, &mvpMatrix[0][0]);
            
            glUniform1f(IL, field.isoLevel);
            glUniform1f(VHL, field.voxelHalfLength);

            glDrawArrays(GL_POINTS, 0, field.voxelCount);
            
            // check and call events and swap the buffers
            glfwPollEvents();
            glfwSwapBuffers(window);
            
            elapsedTime -= 0.013f;
            frame++;
        }
    }

    return 0;
}
