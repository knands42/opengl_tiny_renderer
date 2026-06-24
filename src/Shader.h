#pragma once

#include <sstream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <unordered_map>

class Shader
{
private:
    unsigned int m_RendererID;
    mutable std::unordered_map<const char*, int> m_UniformLocationCache;

public:
    [[nodiscard]] auto GetShaderProgram() const -> unsigned int { return m_RendererID; }

    explicit Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    auto Bind() const -> void;
    auto Unbind() const -> void;
    auto SetUniform4f(const char* name, glm::vec4 vector) const -> void;

private:
    static auto CreateProgram(unsigned int vertexShader, unsigned int fragmentShader) -> unsigned int;
    static auto CompileShader(unsigned int type, const char* source) -> unsigned int;
    static auto ParseShader(const char* file) -> std::string;
    auto GetUniformLocation(const char* name) const -> int;
};
