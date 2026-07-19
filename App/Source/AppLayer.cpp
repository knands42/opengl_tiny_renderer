#include "AppLayer.h"

#include <cassert>

#include <GLFW/glfw3.h>

#include "imgui.h"

namespace App
{
    // TODO: improve, check if is a copy constructor or move constructor
    AppLayer::AppLayer()
        : m_VertexBuffer(Core::VertexBuffer(m_Vertices, sizeof(m_Vertices))), m_VertexArray(Core::VertexArray()),
          m_Shader(Core::Shader("Shaders/vertex.glsl", "Shaders/fragment.glsl")),
          m_Texture(Core::Texture("Textures/wall.jpg")), m_Camera(Core::Camera())
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));

        Core::VertexBufferLayout layout;
        layout.Push<float>(3); // positions
        layout.Push<float>(2); // textures
        m_VertexArray.AddBuffer(m_VertexBuffer, layout);

        m_Shader.Bind();
        m_Texture.Bind();
        m_Shader.SetUniform1i("u_Texture", 0);
        m_Camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        m_Renderer = Core::Renderer();
    }

    void AppLayer::OnEvent(Core::Event& event)
    {
        m_Camera.RaiseEvent(event);
    }

    void AppLayer::OnUpdate(float ts)
    {
    }

    void AppLayer::OnRender()
    {
        Core::ModelMatrix modelMatrix;

        // ImGui UI
        ImGui::SliderFloat3("translation", &modelMatrix.translation.x, -1.5f, 1.5f, "%.3f");
        ImGui::SliderFloat3("scale", &modelMatrix.scale.x, 0.1f, 2.0f, "%.3f");
        ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);

        // render
        glm::vec2 framebufferSize = m_Window->GetFrameBufferSize();
        m_Renderer.Clear();
        m_Shader.Bind();
        m_Shader.SetUniformMat4f("u_Model", m_Camera.GetModelMatrix(modelMatrix));
        m_Shader.SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
        m_Shader.SetUniformMat4f("u_Projection",
                                 m_Camera.GetProjectionMatrix(framebufferSize.x, framebufferSize.y, 0.1f, 100.0f));

        m_Renderer.Draw(m_VertexArray, m_Shader, m_Texture);
    }

    auto AppLayer::GetTime() -> float
    {
        return static_cast<float>(glfwGetTime());
    }
} // namespace App
