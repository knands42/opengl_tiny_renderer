#pragma once

#include "glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "VertexBuffer.h"

class Application
{
public:
    Application();
    ~Application();

    auto run(unsigned int get_shader_program, unsigned int get_vao) -> void;
private:
    unsigned int width = 800, height = 600;
    GLFWwindow *window = nullptr;
    unsigned int m_VAO;
    unsigned int m_shaderProgram;

    void mainLoop() const;

    // glfw: whenever the window size changed (by OS or user resize) this callback
    // function executes
    // ---------------------------------------------------------------------------------------------
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    // process all input: query GLFW whether relevant keys are pressed/released this
    // frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    static void processInput(GLFWwindow *window);
};
