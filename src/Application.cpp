#include "Application.h"

#include "print"

Application::~Application()
{
    glfwTerminate();
}

Application::Application()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::println("Failed to initialize GLAD");
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

auto Application::run(unsigned int shaderProgram, unsigned int VAO) -> void
{
    m_shaderProgram = shaderProgram;
    m_VAO = VAO;

    mainLoop();
    glfwTerminate();
}

void Application::mainLoop() const
{
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw a triangle
        glUseProgram(m_shaderProgram);
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::framebuffer_size_callback(GLFWwindow *window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void Application::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
