#include "Shader.h"

#include <fstream>
#include <print>
#include <utility>
#include <system_error>
#include <filesystem>
#include <GLFW/glfw3.h>

#include "Renderer.h"

Shader::Shader(
    const char *vertexPath,
    const char *fragmentPath)
{
    const std::string vertexShaderSource = ParseShader(vertexPath);
    const std::string fragmentShaderSource = ParseShader(fragmentPath);

    const unsigned int vertexShader =
            CompileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
    const unsigned int fragmentShader =
            CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

    const unsigned int shaderProgram = CreateProgram(vertexShader, fragmentShader);
    m_RendererID = shaderProgram;

    // delete the shaders as they're linked into our program and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

auto Shader::Bind() const -> void
{
    GLCall(glUseProgram(m_RendererID));
}

auto Shader::Unbind() const -> void
{
    GLCall(glUseProgram(0));
}

auto Shader::SetUniform4f(const char *name, glm::vec4 vector) const -> void
{
    const float timeValue = glfwGetTime();
    const float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    const int vertexColorLocation = GetUniformLocation(name);
    ASSERT(vertexColorLocation != -1);
    GLCall(glUniform4f(vertexColorLocation, vector.x, greenValue, vector.z, vector.w));
}

auto Shader::GetUniformLocation(const char* name) const -> unsigned int
{
    if (m_UniformLocationCache.contains(name))
    {
        return m_UniformLocationCache.at(name);
    }

    GLCall(const unsigned int location = glGetUniformLocation(m_RendererID, name));
    if (std::cmp_equal(location , -1))
    {
        printf("ERROR::SHADER::UNIFORM::LOCATION_NOT_FOUND: %s\n", name);
    }

    m_UniformLocationCache[name] = location;

    return location;
}

auto Shader::CompileShader(const unsigned int type, const char* source) -> unsigned int
{
    const unsigned int newShader = glCreateShader(type);
    glShaderSource(newShader, 1, &source, nullptr);
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

auto Shader::ParseShader(const char *file) -> std::string
{
    std::ifstream stream(file);
    if (!stream.is_open())
    {
        throw std::system_error(errno, std::generic_category(), std::string("Failed to open shader: ") + file);
    }

    std::stringstream ss;
    ss << stream.rdbuf();
    return ss.str();
}
