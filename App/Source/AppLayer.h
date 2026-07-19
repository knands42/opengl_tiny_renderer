#pragma once

#include <glad/gl.h>

#include "Camera.h"
#include "Events/Event.h"
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
        Core::VertexBuffer m_VertexBuffer; // MUST outlive m_VertexArray: the VAO records
                                           // attribute pointers into this buffer. If it is
                                           // destroyed, the VAO's attribs dangle -> driver crash.
        Core::VertexArray m_VertexArray;
        Core::Texture m_Texture;
        Core::Camera m_Camera;
        Core::Renderer m_Renderer;

        // Static vertex data (positions + texture coords)
        static constexpr float m_Vertices[] = {
            // positions            // texture coords
            // Front face
            0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            // Right face
            0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            // Back face
            0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            // Left face
            0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            // Bottom face (two triangles)
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        };
    };
} // namespace App
