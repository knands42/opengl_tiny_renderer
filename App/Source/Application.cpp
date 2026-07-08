#include "Application.h"

#include <cassert>
#include <cstdio>
#include <memory>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Window.h"

namespace App
{

    static Application *s_Application = nullptr;

    static void GLFWErrorCallback(int error, const char *description)
    {
        fprintf(stderr, "[GLFW Error]: %s\n", description);
    }

    Application::~Application()
    {
        // TODO: Do not use heap allocation
        delete m_VertexBuffer;
        delete m_VertexArray;
        delete m_Shader;
        delete m_Texture;
        delete m_Camera;

        m_Window->Destroy();

        glfwTerminate();

        s_Application = nullptr;
    }

    Application::Application(const ApplicationSpecification& spec) : m_Specification(spec)
    {
        s_Application = this;

        glfwSetErrorCallback(GLFWErrorCallback);
        glfwInit();

        if (m_Specification.WindowSpec.Title == nullptr)
        {
            m_Specification.WindowSpec.Title = spec.Name;
        }

        m_Window = std::make_shared<Core::Window>(m_Specification.WindowSpec);
        m_Window->Create();

        // Creating VBO, VAO, EBO, Shaders
        constexpr float vertices[] = {
            // positions            // texture coords
            // Front face
            0.0f,
            0.5f,
            0.0f,
            0.5f,
            1.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            // Right face
            0.0f,
            0.5f,
            0.0f,
            0.5f,
            1.0f,
            0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            0.0f,
            // Back face
            0.0f,
            0.5f,
            0.0f,
            0.5f,
            1.0f,
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            0.0f,
            // Left face
            0.0f,
            0.5f,
            0.0f,
            0.5f,
            1.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            // Bottom face (two triangles)
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            1.0f,
        };

        GLCall(glEnable(GL_BLEND)) GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

            GLCall(glEnable(GL_DEPTH_TEST))

                auto *vertexBuffer = new Core::VertexBuffer(vertices, sizeof(vertices));
        m_VertexBuffer = vertexBuffer;

        Core::VertexBufferLayout layout;
        layout.Push<float>(3); // positions
        layout.Push<float>(2); // textures

        Core::VertexArray *vertexArray = new Core::VertexArray();
        m_VertexArray = vertexArray;
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        Core::Shader *shader = new Core::Shader("Shaders/vertex.glsl", "Shaders/fragment.glsl");
        shader->Bind();
        m_Shader = shader;

        Core::Texture *texture = new Core::Texture("Textures/wall.jpg");
        texture->Bind();
        shader->SetUniform1i("u_Texture", 0);
        m_Texture = texture;

        Core::Camera *camera = new Core::Camera();
        camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        m_Camera = camera;

        constexpr Core::Renderer renderer;
        m_Renderer = renderer;
    }

    auto Application::Stop() -> void
    {
        m_Running = false;
    }

    auto Application::Run() -> void
    {
        m_Running = true;
        float lastTime = GetTime();

        while (m_Running)
        {
            if (m_Window->ShouldClose())
            {
                Stop();
                break;
            }

            float currentTime = GetTime();
            float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
            lastTime = currentTime;

            // render
            m_Renderer.Clear();
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_Model", m_Camera->GetModelMatrix());
            m_Shader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix(glm::vec3(0.0f)));
            m_Shader->SetUniformMat4f("u_Projection",
                                      m_Camera->GetProjectionMatrix(glm::radians(45.0f), static_cast<float>(width),
                                                                    static_cast<float>(height), 0.1f, 100.0f));

            m_Renderer.Draw(*m_VertexArray, *m_Shader, *m_Texture);

            // check and call events and swap the buffers
            m_Window->Update();
        }
    }

    glm::vec2 Application::GetFramebufferSize() const
    {
        return m_Window->GetFrameBufferSize();
    }

    Application& Application::Get()
    {
        assert(s_Application);
        return *s_Application;
    }

    float Application::GetTime()
    {
        return static_cast<float>(glfwGetTime());
    }
} // namespace App
