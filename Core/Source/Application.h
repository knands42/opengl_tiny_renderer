#include <memory>

#include <glad/gl.h>

#include "Camera.h"
#include "Events/Event.h"
#include "Layer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertices/VertexArray.h"
#include "Window.h"

namespace Core
{
    struct ApplicationSpecification
    {
        const char *Name = "OpenGL Tiny Renderer";
        WindowSpecification WindowSpec;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& spec = ApplicationSpecification());
        ~Application();

        auto Run() -> void;
        auto Stop() -> void;

        [[nodiscard]] auto GetFramebufferSize() const -> glm::vec2;
        static auto Get() -> Application&;
        auto GetWindow()
        {
            return m_Window;
        }

        static auto GetTime() -> float;

        template <typename TLayer>
            requires(std::is_base_of_v<Layer, TLayer>)
        void PushLayer()
        {
            m_LayerStack.push_back(std::make_unique<TLayer>());
        }

        void RaiseEvent(Event& event);

    private:
        ApplicationSpecification m_Specification;
        std::shared_ptr<Window> m_Window;
        bool m_Running = false;

        mutable std::vector<std::unique_ptr<Layer>> m_LayerStack;
    };
} // namespace Core
