#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex shader
const char* vertex_shader =
    "#version 400\n"
    "in vec3 vp;"
    "void main() {"
    "  gl_Position = vec4(vp, 1.0);"
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
    // Normalized device coordinates
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0,
        0.0f, 0.5f, 0.0f
    };

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
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}
