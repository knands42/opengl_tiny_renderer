#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>

#include "Events/Event.h"
#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"

namespace Core
{
    struct ModelMatrix
    {
        glm::vec3 translation = glm::vec3(0.5f, 0.5f, 0.0f);
        glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.5f);
    };

    class Camera
    {
    private:
        float m_AspectRatio; // TODO: how to use this?
        float m_Fov = 45.0f;

        float m_LastFrame = 0.0f;

        glm::vec3 m_Position;
        glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        float m_LastX = 400;
        float m_LastY = 300;
        float m_CurrentYaw = 0.0f;
        float m_CurrentPitch = 0.0f;

        void RecomputeViewMatrix(float yaw, float pitch);

        static bool HandleKeyPressedEvent(Camera& camera, KeyPressedEvent& event);
        static bool HandleMouseMovedEvent(Camera& camera, MouseMovedEvent& event);
        static bool HandleWindowScrollEvent(Camera& camera, WindowScrollEvent& event);

    public:
        Camera();
        ~Camera();

        [[nodiscard]] glm::mat4 GetViewMatrix() const;
        [[nodiscard]] glm::mat4 GetProjectionMatrix(float width, float height, float nearPlane, float farPlane) const;
        [[nodiscard]] glm::mat4 GetModelMatrix(ModelMatrix& modelMatrix) const;
        void RaiseEvent(Event& event);

        void SetPosition(glm::vec3 position);
    };
} // namespace Core
