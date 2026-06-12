#pragma once

#include <sstream>

class shader
{
public:
    unsigned int ShaderProgram(unsigned int vertexShader,
                               unsigned int fragmentShader);

    unsigned int CompileShader(const unsigned int type, const std::string &source);
};
