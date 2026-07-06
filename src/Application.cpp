#include "Application.h"

#include "print"
#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        // positions            // texture coords
        // Front face
         0.0f,  0.5f,  0.0f,   0.5f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
        // Right face
         0.0f,  0.5f,  0.0f,   0.5f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
        // Back face
         0.0f,  0.5f,  0.0f,   0.5f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
        // Left face
         0.0f,  0.5f,  0.0f,   0.5f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
        // Bottom face (two triangles)
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
    };

    GLCall(glEnable(GL_BLEND))
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

    GLCall(glEnable(GL_DEPTH_TEST))

    auto *vertexBuffer = new VertexBuffer(vertices, sizeof(vertices));
    m_VertexBuffer = vertexBuffer;

    VertexBufferLayout layout;
    layout.Push<float>(3); // positions
    layout.Push<float>(2); // textures

    VertexArray *vertexArray = new VertexArray();
    m_VertexArray = vertexArray;
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

    Shader *shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    shader->Bind();
    m_Shader = shader;

    Texture *texture = new Texture("textures/wall.jpg");
    texture->Bind();
    shader->SetUniform1i("u_Texture", 0);
    m_Texture = texture;

    Camera *camera = new Camera();
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_Camera = camera;

    constexpr Renderer renderer;
    m_Renderer = renderer;
}

auto Application::Run() const -> void
{
    MainLoop();

    // TODO: Do not use heap allocation
    delete m_VertexBuffer;
    delete m_VertexArray;
    delete m_Shader;
    delete m_Texture;
    delete m_Camera;

    glfwTerminate();
}

void Application::MainLoop() const
{
    while (!glfwWindowShouldClose(window))
    {
        // input
        ProcessInput(window);

        // rendering commands here
        m_Renderer.Clear();

        // model: rotate continuously around Z over time
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", m_Camera->GetModelMatrix());
        m_Shader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix(glm::vec3(0.0f)));
        m_Shader->SetUniformMat4f("u_Projection", m_Camera->GetProjectionMatrix(
                                      glm::radians(45.0f), static_cast<float>(width), static_cast<float>(height), 0.1f,
                                      100.0f));

        // render
        m_Renderer.Draw(*m_VertexArray, *m_Shader, *m_Texture);

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
