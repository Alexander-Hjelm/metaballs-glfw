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

// Geometry shader
const char* geometry_shader =
    "#version 400\n"
    "layout(triangles) in;"
    "layout(triangle_strip, max_vertices = 3) out;"
    "void main() {"
    "    for(int i = 0; i < gl_in.length(); ++i)"
    "    {"
    "       gl_Position = gl_in[i].gl_Position;"
    "       EmitVertex();"
    "    }"
    "    EndPrimitive();"
    "}";

// Fragment shader
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

void CheckShaderCompilationError(const char* shaderName, unsigned int shaderID)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << shaderName << " failed to compile :\n" << infoLog << std::endl;
    };
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
    0.5f, 0.5f, -0.5f, // top right
    0.5f, -0.5f, -0.5f, // bottom right
    -0.5f, -0.5f, -0.5f, // bottom left
    -0.5f, 0.5f, -0.5f, // top left
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f
    };
    unsigned int indices[] = { 
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
    4, 5, 7,
    5, 6, 7,
    0, 1, 4,
    1, 4, 5,
    1, 2, 5,
    2, 5, 6,
    2, 3, 6,
    3, 6, 7,
    3, 0, 7,
    0, 7, 4
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
    CheckShaderCompilationError("Vertex Shader", VS);
    unsigned int GS = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(GS, 1, &geometry_shader, NULL);
    glCompileShader(GS);
    CheckShaderCompilationError("Geometry Shader", GS);
    unsigned int FS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FS, 1, &fragment_shader, NULL);
    glCompileShader(FS);
    CheckShaderCompilationError("Fragment Shader", FS);
    
    // Create the shader program
    unsigned int SHADER_PROGRAM = glCreateProgram();
    glAttachShader(SHADER_PROGRAM, VS);
    glAttachShader(SHADER_PROGRAM, GS);
    glAttachShader(SHADER_PROGRAM, FS);
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

        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
        
        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}
