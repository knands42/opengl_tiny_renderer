#pragma once

#include <memory>

#include <glad/gl.h>

#include "Camera.h"
#include "Events/Event.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Window.h"

namespace App
{
    struct ApplicationSpecification
    {
        const char *Name = "OpenGL Tiny Renderer";
        Core::WindowSpecification WindowSpec;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& spec = ApplicationSpecification());
        ~Application();

        auto Run() -> void;
        auto Stop() -> void;

        glm::vec2 GetFramebufferSize() const;
        static Application& Get();
        static float GetTime();

        void RaiseEvent(Core::Event& event);

    private:
        ApplicationSpecification m_Specification;
        std::shared_ptr<Core::Window> m_Window;
        bool m_Running = false;

        // TODO: Remove heap allocation
        Core::Shader *m_Shader{};
        Core::VertexBuffer *m_VertexBuffer{};
        Core::IndexBuffer *m_IndexBuffer{};
        Core::VertexArray *m_VertexArray{};
        Core::Texture *m_Texture{};
        Core::Camera *m_Camera{};
        Core::Renderer m_Renderer;
    };
} // namespace App
