#include "shader.h"

#include <glad/glad.h>
#include <fstream>
#include <string>
#include <iostream>

Shader::Shader()
{
    // Create shader object
    VS = glCreateShader(GL_VERTEX_SHADER);
    GS = glCreateShader(GL_GEOMETRY_SHADER);
    FS = glCreateShader(GL_FRAGMENT_SHADER);
    Program = glCreateProgram();
    
    // Compile shaders
    RecompileAndLink();
}

Shader::~Shader()
{
    glDeleteShader(VS);
    glDeleteShader(GS);
    glDeleteShader(FS);
    glDeleteProgram(Program);
}

void Shader::CompileShader(unsigned int shaderID, const char* shaderPath)
{
    std::string shaderSource;
    std::ifstream file(shaderPath);
    if(file.is_open())
    {
        std::string line;
        while(getline(file, line))
            shaderSource += line + '\n';
        file.close();
    }
    
    const char* shaderSourceCStr = shaderSource.c_str();
    glShaderSource(shaderID, 1, &shaderSourceCStr, NULL);
    glCompileShader(shaderID);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << shaderPath << " failed to compile :\n" << infoLog << std::endl;
    };
}

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
