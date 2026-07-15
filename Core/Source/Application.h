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

        static auto GetTime() -> float;

        void RaiseEvent(Event& event);

    private:
        ApplicationSpecification m_Specification;
        std::shared_ptr<Window> m_Window;
        bool m_Running = false;

        std::array<std::unique_ptr<Layer>, 2> m_LayerStack;

        // TODO: Remove heap allocation
        Shader *m_Shader{};
        VertexBuffer *m_VertexBuffer{};
        IndexBuffer *m_IndexBuffer{};
        VertexArray *m_VertexArray{};
        Texture *m_Texture{};
        Camera *m_Camera{};
        Renderer m_Renderer;
    };
} // namespace Core
