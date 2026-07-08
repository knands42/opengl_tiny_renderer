#pragma once

#include <stdint.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Core
{

    class Window
    {
    public:
        struct WindowSpecification
        {
            const char *Title;
            uint32_t Width = 1280;
            uint32_t Height = 720;
            bool IsResizable = true;
            bool VSync = true;
        };

        Window(const WindowSpecification& specification = WindowSpecification());

        ~Window();

        void Create();

        void Destroy();

        bool ShouldClose() const;

    private:
        WindowSpecification m_Specification;
        GLFWwindow *m_Window = nullptr;

        void ProcessInput(GLFWwindow *window);
    };
} // namespace Core
