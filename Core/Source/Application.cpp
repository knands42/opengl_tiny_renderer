#include "Application.h"

#include <cassert>
#include <memory>
#include <print>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

            // TODO: Iterate over layers and call them

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
        for (auto& layer : std::views::reverse(m_LayerStack))
        {
            layer->OnEvent(event);
            if (event.Handled)
                break;
        }
    }
} // namespace Core
