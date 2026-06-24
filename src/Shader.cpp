#include "Shader.h"

#include <fstream>
#include <print>
#include <utility>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <GLFW/glfw3.h>

#include "Renderer.h"

Shader::Shader(
    const std::string &vertexPath,
    const std::string &fragmentPath)
{
    const std::string vertexShaderSource = ParseShader(vertexPath);
    const std::string fragmentShaderSource = ParseShader(fragmentPath);

    const unsigned int vertexShader =
            CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    const unsigned int fragmentShader =
            CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_VertexShader = vertexShader;
    m_FragmentShader = fragmentShader;

    const unsigned int shaderProgram = CreateProgram(m_VertexShader, m_FragmentShader);
    m_ShaderProgram = shaderProgram;

    // delete the shaders as they're linked into our program and no longer necessary
    glDeleteShader(m_VertexShader);
    glDeleteShader(m_FragmentShader);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_ShaderProgram));
}

auto Shader::Bind() const -> void
{
    GLCall(glUseProgram(m_ShaderProgram));
}

auto Shader::Unbind() const -> void
{
    GLCall(glUseProgram(0));
}

auto Shader::SetUniform(const char *variable, glm::vec4 vector) const -> void
{
    const float timeValue = glfwGetTime();
    const float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    const int vertexColorLocation = glGetUniformLocation(m_ShaderProgram, "ourColor");
    ASSERT(vertexColorLocation != -1);
    GLCall(glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f));
}

auto Shader::CompileShader(const unsigned int type, const std::string &source) -> unsigned int
{
    const unsigned int newShader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(newShader, 1, &src, nullptr);
    GLCall(glCompileShader(newShader));

    // check for shader compile errors
    int success = 0;
    glGetShaderiv(newShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int length = 0;
        glGetShaderiv(newShader, GL_INFO_LOG_LENGTH, &length);
        char infoLog[length];
        glGetShaderInfoLog(newShader, 512, NULL, infoLog);
        std::println("ERROR::SHADER::VERTEX::COMPILATION_FAILED: {}", *infoLog);
    }

    return newShader;
}

auto Shader::CreateProgram(const unsigned int vertexShader, const unsigned int fragmentShader) -> unsigned int
{
    const unsigned int shaderProgram = glCreateProgram();
    GLCall(glAttachShader(shaderProgram, vertexShader));
    GLCall(glAttachShader(shaderProgram, fragmentShader));
    GLCall(glLinkProgram(shaderProgram));
    GLCall(glValidateProgram(shaderProgram));

    // check for linking errors
    int success = 0;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
        std::println("ERROR::SHADER::PROGRAM::LINKING_FAILED: {}", *infoLog);
    }

    return shaderProgram;
}

auto Shader::ParseShader(const std::string &file) -> std::string
{
    std::ifstream stream(file);
    if (!stream.is_open())
    {
        throw std::runtime_error("Failed to open shader: " + file + " with error: " + std::strerror(errno));
    }

    std::stringstream ss;
    ss << stream.rdbuf();
    return ss.str();
}
