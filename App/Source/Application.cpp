#include "Application.h"

#include <cassert>
#include <cstdio>
#include <memory>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Events/WindowEvents.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace App
{

    static Application *s_Application = nullptr;

    static void GLFWErrorCallback(int error, const char *description)
    {
        fprintf(stderr, "[GLFW Error]: %s\n", description);
    }

    Application::~Application()
    {
        Stop();

        delete m_VertexBuffer;
        delete m_IndexBuffer;
        delete m_VertexArray;
        delete m_Texture;
        delete m_Camera;
        delete m_Shader;

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

        m_Specification.WindowSpec.EventCallback = [this](Core::Event& event) { RaiseEvent(event); };

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

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));

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

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 130");
    }

    auto Application::Stop() -> void
    {
        m_Running = false;
    }

    auto Application::Run() -> void
    {
        m_Running = true;
        float lastTime = GetTime();
        Core::ModelMatrix modelMatrix;

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

            // --- begin ImGui frame ---
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // ImGui UI
            ImGui::SliderFloat3("translation", &modelMatrix.translation.x, -1.0f, 1.0f, "%.3f");
            ImGui::SliderFloat3("scale", &modelMatrix.scale.x, 0.1f, 2.0f, "%.3f");
            ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);

            ImGui::Render();

            // render
            glm::vec2 framebufferSize = m_Window->GetFrameBufferSize();
            m_Renderer.Clear();
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_Model", m_Camera->GetModelMatrix(modelMatrix));
            m_Shader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
            m_Shader->SetUniformMat4f(
                "u_Projection",
                m_Camera->GetProjectionMatrix(framebufferSize.x, framebufferSize.y, 0.1f, 100.0f));

            m_Renderer.Draw(*m_VertexArray, *m_Shader, *m_Texture);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

    void Application::RaiseEvent(Core::Event& event)
    {
        m_Camera->RaiseEvent(event);
        Core::EventDispatcher dispatcher(event);
    }
} // namespace App
