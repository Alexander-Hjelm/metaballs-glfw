#include "shader.h"
#include "common.h"

#include <glad/glad.h>
#include <fstream>
#include <string>
#include <iostream>

/**
 *  Constructor
 */
Shader::Shader()
{
    // Create shader object
    VS = glCreateShader(GL_VERTEX_SHADER);
    GS = glCreateShader(GL_GEOMETRY_SHADER);
    FS = glCreateShader(GL_FRAGMENT_SHADER);
    Program = glCreateProgram();
    
    // Compile shaders
    RecompileAndLink();
    
    // Bind shader
    glUseProgram(Program);
}

/**
 *  Destructor
 */
Shader::~Shader()
{
    // Clean up work
    glDeleteShader(VS);
    glDeleteShader(GS);
    glDeleteShader(FS);
    glDeleteProgram(Program);
}

/**
 *  Compile shader from source file
 *  @param shaderID   - OpenGL shader reference identifier
 *  @param shaderPath - The relative path to the source file
 */
void Shader::CompileShader(unsigned int shaderID, const char* shaderPath)
{
    // Read shader source from file
    std::string shaderSource;
    std::ifstream file(shaderPath);
    if(file.is_open())
    {
        std::string line;
        while(getline(file, line))
        {
            std::string searchWord = "METABALL_COUNT";
            size_t foundPos = line.find(searchWord);
            if(foundPos != std::string::npos)
            {
                line.replace(foundPos, searchWord.size(), std::to_string(METABALL_COUNT));
            }
            shaderSource += line + '\n';
        }
        file.close();
    }
    else
    {
        std::cout << "Could not open file " << shaderPath << ", please check the path!" << std::endl;
    }
    
    // Compile
    const char* shaderSourceCStr = shaderSource.c_str();
    glShaderSource(shaderID, 1, &shaderSourceCStr, NULL);
    glCompileShader(shaderID);
    
    // Query for compilation error message
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << shaderPath << " failed to compile :\n" << infoLog << std::endl;
    };
}

/**
 *  Compile shader and link all shaders to shader program
 */
void Shader::RecompileAndLink()
{
    // Compile shaders
    CompileShader(VS, "../shader/vertex_shader.glsl");
    CompileShader(GS, "../shader/geometry_shader.glsl");
    CompileShader(FS, "../shader/fragment_shader.glsl");
    
    // Link to shader program
    glAttachShader(Program, VS);
    glAttachShader(Program, GS);
    glAttachShader(Program, FS);
    glLinkProgram(Program);
    
    std::cout << "Shader compiled!" << std::endl;
}
