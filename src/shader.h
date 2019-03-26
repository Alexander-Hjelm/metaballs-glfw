#pragma once

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
};
