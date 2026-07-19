#include "Application.h"

#include <cassert>
#include <memory>
#include <print>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ranges"

namespace Core
{
    static Application *s_Application = nullptr;

    static void GLFWErrorCallback(int error, const char *description)
    {
        std::println(stderr, "[GLFW Error]: {}", description);
    }

    Application::~Application()
    {
        Stop();

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

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            float currentTime = GetTime();
            float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
            lastTime = currentTime;

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
            {
                layer->OnUpdate(timestep);
            }

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
            {
                layer->OnRender();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // check and call events and swap the buffers
            m_Window->Update();
        }
    }

    auto Application::GetFramebufferSize() const -> glm::vec2
    {
        return m_Window->GetFrameBufferSize();
    }

    auto Application::Get() -> Application&
    {
        assert(s_Application);
        return *s_Application;
    }

    auto Application::GetTime() -> float
    {
        return static_cast<float>(glfwGetTime());
    }

    void Application::RaiseEvent(Core::Event& event)
    {
        if (event.GetEventType() == EventType::WindowClose)
        {
            Stop();
            event.Handled = true;
            return;
        }

        for (auto& layer : std::views::reverse(m_LayerStack))
        {
            layer->OnEvent(event);
            if (event.Handled)
                break;
        }

        return;
    }
} // namespace Core
