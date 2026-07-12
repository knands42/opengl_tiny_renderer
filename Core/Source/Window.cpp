#include "Window.h"

#include <cassert>
#include <print>

#include <GLFW/glfw3.h>

#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"
#include "glad/gl.h"

namespace Core
{

    Window::Window(const WindowSpecification& specification) : m_Specification(specification)
    {
    }

    Window::~Window()
    {
        Destroy();
    }

    void Window::Create()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        m_Window = glfwCreateWindow(static_cast<int>(m_Specification.Width), static_cast<int>(m_Specification.Height),
                                    m_Specification.Title, nullptr, nullptr);
        if (m_Window == nullptr)
        {
            std::println("Failed to create GLFW window");
            assert(false);
        }

        glfwMakeContextCurrent(m_Window);
        glfwSetFramebufferSizeCallback(m_Window, OnWindowResize);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGL(glfwGetProcAddress))
        {
            std::println("Failed to initialize GLAD");
            assert(false);
        }

        glfwSwapInterval(m_Specification.VSync ? 1 : 0);
        glfwSetWindowUserPointer(m_Window, this);

        // set initial viewport
        int fbW = 0, fbH = 0;
        glfwGetFramebufferSize(m_Window, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);
        
        // handle events
        glfwSetWindowCloseCallback(m_Window, OnWindowClose);

        // handle inputs
        // ---------------------------------------
        ProcessInput(m_Window);

        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(m_Window, OnMouseCallback);
    }

    void Window::Destroy()
    {
        if (m_Window)
        {
            glfwDestroyWindow(m_Window);

            m_Window = nullptr;
        }
    }

    void Window::Update()
    {
        ProcessInput(m_Window);
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    auto Window::ShouldClose() const -> bool
    {
        return glfwWindowShouldClose(m_Window) != 0;
    }

    void Window::RaiseEvent(Event& event)
    {
        if (m_Specification.EventCallback)
        {
            m_Specification.EventCallback(event);
        }
    }

    auto Window::GetFrameBufferSize() const -> glm::vec2
    {
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return {width, height};
    }

    auto Window::GetMousePos() const -> glm::vec2
    {
        double x, y;
        glfwGetCursorPos(m_Window, &x, &y);
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    void Window::ProcessInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_W))
        {
            KeyPressedEvent event(GLFW_KEY_W, true);
            RaiseEvent(event);
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            KeyPressedEvent event(GLFW_KEY_S, true);
            RaiseEvent(event);
        }
        if (glfwGetKey(window, GLFW_KEY_A))
        {
            KeyPressedEvent event(GLFW_KEY_A, true);
            RaiseEvent(event);
        }
        if (glfwGetKey(window, GLFW_KEY_D))
        {
            KeyPressedEvent event(GLFW_KEY_D, true);
            RaiseEvent(event);
        }
    }

    void Window::OnMouseCallback(GLFWwindow *window, double xpos, double ypos)
    {
        Window& self = *static_cast<Window *>(glfwGetWindowUserPointer(window));
        MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
        self.RaiseEvent(event);
    }

    void Window::OnWindowResize(GLFWwindow *window, int width, int height)
    {
        int fbW = 0, fbH = 0;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);
        
        Window& self = *static_cast<Window *>(glfwGetWindowUserPointer(window));
        WindowResizeEvent event(fbW, fbH);
        self.RaiseEvent(event);
    }

    void Window::OnWindowClose(GLFWwindow *window)
    {
        Window& self = *static_cast<Window *>(glfwGetWindowUserPointer(window));
        WindowClosedEvent event;
        self.RaiseEvent(event);
    }

} // namespace Core
