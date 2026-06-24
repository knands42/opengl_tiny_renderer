#include "Application.h"

#include "print"

Application::~Application()
{
    glfwTerminate();
}

Application::Application()
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
    window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::println("Failed to initialize GLAD");
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Creating VBO, VAO, EBO, Shaders
    constexpr float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
   };
    constexpr unsigned int indices[] = {
        0, 1, 2
    };

    auto* vertexBuffer = new VertexBuffer(vertices, sizeof(vertices));
    m_VertexBuffer = vertexBuffer;

    IndexBuffer* indexBuffer = new IndexBuffer(indices, 3);
    m_IndexBuffer = indexBuffer;

    VertexBufferLayout layout;
    layout.Push<float>(3); // positions
    layout.Push<float>(3); // colors

    VertexArray* vertexArray = new VertexArray();
    m_VertexArray = vertexArray;
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

    Shader* shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    shader->Bind();
    m_Shader = shader;
}

auto Application::Run() const -> void
{
    MainLoop();

    delete m_VertexBuffer;
    delete m_IndexBuffer;
    delete m_VertexArray;
    delete m_Shader;

    glfwTerminate();
}

void Application::MainLoop() const
{
    while (!glfwWindowShouldClose(window))
    {
        // input
        ProcessInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render
        m_Shader->Bind();
        m_VertexArray->Bind();
        m_IndexBuffer->Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr));
        // GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::FramebufferSizeCallback(GLFWwindow *window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void Application::ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
