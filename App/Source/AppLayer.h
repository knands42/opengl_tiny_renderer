#pragma once

#include <glad/gl.h>

#include "Camera.h"
#include "Events/Event.h"
#include "IndexBuffer.h"
#include "Layer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertices/VertexArray.h"
#include "Vertices/VertexBuffer.h"

namespace App
{
    class AppLayer : public Core::Layer
    {
    public:
        AppLayer();
        ~AppLayer() override = default;

        virtual void OnEvent(Core::Event& event) override;
        virtual void OnUpdate(float ts) override;

        virtual void OnRender() override;

        auto GetTime() -> float;

    private:
        Core::Shader m_Shader;
        Core::VertexBuffer m_VertexBuffer;
        Core::IndexBuffer m_IndexBuffer;
        Core::VertexArray m_VertexArray;
        Core::Texture m_Texture;
        Core::Camera m_Camera;
        Core::Renderer m_Renderer;

        float m_LastTime;
    };
} // namespace App
