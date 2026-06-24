#pragma once

#include <sstream>
#include <glad/gl.h>
#include <glm/glm.hpp>

class Shader
{
public:
    [[nodiscard]] auto GetVertexShader() const -> unsigned int { return m_VertexShader; }
    [[nodiscard]] auto GetFragmentShader() const -> unsigned int { return m_FragmentShader; }
    [[nodiscard]] auto GetShaderProgram() const -> unsigned int { return m_ShaderProgram; }

    explicit Shader(const std::string&  vertexPath, const std::string&  fragmentPath);
    ~Shader();

    auto Bind() const -> void;
    auto Unbind() const -> void;
    auto SetUniform(const char* variable, glm::vec4 vector) const -> void;
private:
    unsigned int m_ShaderProgram;
    unsigned int m_VertexShader;
    unsigned int m_FragmentShader;

    static auto CreateProgram(unsigned int vertexShader, unsigned int fragmentShader) -> unsigned int;
    static auto CompileShader(unsigned int type, const std::string &source) -> unsigned int;
    static auto ParseShader(const std::string &file) -> std::string;
};
