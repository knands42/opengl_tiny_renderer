#pragma once

#include <stdint.h>

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>

namespace Core
{
    struct WindowSpecification
    {
        const char *Title;
        uint32_t Width = 1280;
        uint32_t Height = 720;
        bool IsResizable = true;
        bool VSync = true;
    };

    class Window
    {
    public:
        Window(const WindowSpecification& specification = WindowSpecification());
        ~Window();

        void Create();
        void Destroy();
        void Update();
        bool ShouldClose() const;

        glm::vec2 GetFrameBufferSize() const;
        glm::vec2 GetMousePos() const;

    private:
        WindowSpecification m_Specification;
        GLFWwindow *m_Window = nullptr;

        void ProcessInput(GLFWwindow *window);
    };
} // namespace Core
