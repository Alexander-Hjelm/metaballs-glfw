#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Vertex shader
const char* vertex_shader =
    "#version 400\n"
    "in vec3 vp;"
    "uniform mat4 MVP;"  // uniform = value stays the same for the whole mesh
    "void main() {"
    "  gl_Position = MVP * vec4(vp, 1.0);"
    "}";

//Fragment shader
const char* fragment_shader =
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

    // Vertex definitions
    float vertices[] = {
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f // top left
    };
    unsigned int indices[] = { 
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
    };

    // Element buffer objecct (index buffer)
    unsigned int EBO;
    glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex buffer object, gives instant access to vertices in the GPU
    // GenBuffers yields a unique ID for the newly created buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    // Specify type of the vertex buffer and bind it to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy the vertex data to the boud buffer's memory
    // GL_STATIC_DRAW specifies how the GPU should manage the data
    // (static = the data will likely not change)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex array object
    // Holds all pur buffers
    // When we want to render all triangles we can simply bind the vertex array and draw
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Specify the attribute location (0) of the first vertex buffer, and the type of data.
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, NULL);


    // Compile shaders
    unsigned int VS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VS, 1, &vertex_shader, NULL);
    glCompileShader(VS);
    unsigned int FS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FS, 1, &fragment_shader, NULL);
    glCompileShader(FS);
    
    // Create the shader program
    unsigned int SHADER_PROGRAM = glCreateProgram();
    glAttachShader(SHADER_PROGRAM, FS);
    glAttachShader(SHADER_PROGRAM, VS);
    glLinkProgram(SHADER_PROGRAM);


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

    // Get an id for the mvp matrixmvp matrix
    unsigned int MVP_ID = glGetUniformLocation(SHADER_PROGRAM, "MVP");

    // render loop
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
   
        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the triangle
        glUseProgram(SHADER_PROGRAM);

        // Link mvp matrix with the currently boud GLSL shader
        glUniformMatrix4fv(MVP_ID, 1, false, &mvpMatrix[0][0]);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}
