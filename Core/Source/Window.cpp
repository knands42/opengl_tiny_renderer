#include "Window.h"

#include <cassert>
#include <print>

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
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *window, const int width, const int height)
                                       { glViewport(0, 0, width, height); });

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGL(glfwGetProcAddress))
        {
            std::println("Failed to initialize GLAD");
            assert(false);
        }

        glfwSwapInterval(m_Specification.VSync ? 1 : 0);
        glfwSetWindowUserPointer(m_Window, this);

        // handle inputs
        // ---------------------------------------
        ProcessInput(m_Window);
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
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window) != 0;
    }

    glm::vec2 Window::GetFrameBufferSize() const
    {
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return {width, height};
    }

    glm::vec2 Window::GetMousePos() const
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
    }
} // namespace Core
