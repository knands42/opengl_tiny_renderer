#pragma once

#include <functional>
#include <stdint.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Events/Event.h"

namespace Core
{
    struct WindowSpecification
    {
        const char *Title;
        uint32_t Width = 1280;
        uint32_t Height = 720;
        bool IsResizable = true;
        bool VSync = true;

        using EventCallbackFn = std::function<void(Event& e)>;
        EventCallbackFn EventCallback;
    };

    class Window
    {
    public:
        Window(const WindowSpecification& specification = WindowSpecification());
        ~Window();

        void Create();
        void Destroy();
        void Update();
        [[nodiscard]] auto ShouldClose() const -> bool;

        [[nodiscard]] auto GetNativeWindow() const -> GLFWwindow *
        {
            return m_Window;
        }

        void RaiseEvent(Event& event);

        [[nodiscard]] auto GetFrameBufferSize() const -> glm::vec2;
        [[nodiscard]] auto GetMousePos() const -> glm::vec2;

    private:
        WindowSpecification m_Specification;
        GLFWwindow *m_Window = nullptr;

        void ProcessInput(GLFWwindow *window);
        static void OnMouseCallback(GLFWwindow *window, double xpos, double ypos);
        static void OnWindowResize(GLFWwindow *window, int width, int height);
        static void OnWindowClose(GLFWwindow *window);
        static void OnWindowScroll(GLFWwindow *window, double xoffset, double yoffset);
    };
} // namespace Core
