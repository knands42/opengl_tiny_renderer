#include "Window.h"

#include <print>

namespace Core
{

    Window::Window(const WindowSpecification& specification)
    {
    }

    Window::~Window()
    {
        Destroy();
    }

    void Window::Create()
    {
        glfwInit();
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
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_Window);
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *window, const int width, const int height)
                                       { glViewport(0, 0, width, height); });

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGL(glfwGetProcAddress))
        {
            std::println("Failed to initialize GLAD");
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glfwSwapInterval(m_Specification.VSync ? 1 : 0);
        glfwSetWindowUserPointer(m_Window, this);
    }

    void Window::Destroy()
    {
    }

    bool Window::ShouldClose() const
    {
    }

    void Window::ProcessInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

} // namespace Core
