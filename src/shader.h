#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>

struct Shader
{
public:
    unsigned int VS;
    unsigned int GS;
    unsigned int FS;
    unsigned int Program;
public:
    Shader();
    ~Shader();
    void CompileShader(unsigned int shaderID, const char* shaderPath);
    void RecompileAndLink();
    
    inline void SetUniform(const char* name, float f)
    {
        unsigned int location = glGetUniformLocation(Program, name);
        if(location == -1) { std::cout << "Uniform " << name << " not found!" << std::endl; return; }
        glUniform1f(location, f);
    }
    inline void SetUniform(const char* name, unsigned int i)
    {
        unsigned int location = glGetUniformLocation(Program, name);
        if(location == -1) { std::cout << "Uniform " << name << " not found!" << std::endl; return; }
        glUniform1i(location, i);
    }
};
