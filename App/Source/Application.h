#pragma once

#include <glad/gl.h>

#include "Camera.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

constexpr unsigned int DEFAULT_WIDTH = 800;
constexpr unsigned int DEFAULT_HEIGHT = 600;

namespace App
{
    class Application
    {
    public:
        Application();
        ~Application();

        auto Run() const -> void;

    private:
        unsigned int width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
        GLFWwindow *window = nullptr;
        Core::Shader *m_Shader{};
        Core::VertexBuffer *m_VertexBuffer{};
        Core::IndexBuffer *m_IndexBuffer{};
        Core::VertexArray *m_VertexArray{};
        Core::Texture *m_Texture{};
        Core::Camera *m_Camera{};
        Core::Renderer m_Renderer;

        void MainLoop() const;

        // glfw: whenever the window size changed (by OS or user resize) this callback
        // function executes
        // ---------------------------------------------------------------------------------------------
        static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);

        // process all input: query GLFW whether relevant keys are pressed/released this
        // frame and react accordingly
        // ---------------------------------------------------------------------------------------------------------
        static void ProcessInput(GLFWwindow *window);
    };
} // namespace App
