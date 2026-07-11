#include "Camera.h"

#include <cstdlib>

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/trigonometric.hpp>

namespace Core
{
    Camera::Camera() : m_Position(glm::vec3(0.0f))
    {
    }

    Camera::~Camera()
    {
    }

    auto Camera::GetViewMatrix() const -> glm::mat4
    {
        return glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);
    }

    auto Camera::GetProjectionMatrix(const float fov, const float width, const float height, const float nearPlane,
                                     const float farPlane) const -> glm::mat4
    {
        return glm::perspective(fov, width / height, nearPlane, farPlane);
    }

    auto Camera::GetModelMatrix(ModelMatrix& modelMatrix) const -> glm::mat4
    {
        auto transform = glm::mat4(1.0f);
        transform = glm::translate(transform, modelMatrix.translation);
        transform = glm::rotate(transform, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));
        transform = glm::scale(transform, modelMatrix.scale);
        return transform;
    }

    void Camera::SetPosition(const glm::vec3 position)
    {
        m_Position = position;
    }

    void Camera::RaiseEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) -> bool
                                             { return HandleKeyPressedEvent(*this, e); });
        dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e) -> bool
                                             { return HandleMouseMovedEvent(*this, e); });
    }

    auto Camera::HandleKeyPressedEvent(Camera& camera, KeyPressedEvent& event) -> bool
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - camera.m_LastFrame;
        camera.m_LastFrame = currentFrame;

        const float cameraSpeed = 0.05f;
        const glm::vec3 cameraFrontTranslation = camera.m_CameraFront * cameraSpeed;
        const glm::vec3 cameraRightTranslation =
            glm::normalize(glm::cross(camera.m_CameraFront, camera.m_CameraUp)) * cameraSpeed;

        switch (event.GetKeyCode())
        {
        case GLFW_KEY_W:
            camera.m_Position += cameraFrontTranslation;
            break;
        case GLFW_KEY_S:
            camera.m_Position -= cameraFrontTranslation;
            break;
        case GLFW_KEY_A:
            camera.m_Position += cameraRightTranslation;
            break;
        case GLFW_KEY_D:
            camera.m_Position -= cameraRightTranslation;
            break;
        default:
            return false;
        }

        return true;
    }

    auto Camera::HandleMouseMovedEvent(Camera& camera, MouseMovedEvent& event) -> bool
    {
        float xoffset = static_cast<float>(event.GetX()) - camera.m_LastX;
        float yoffset = static_cast<float>(event.GetY()) - camera.m_LastY;
        camera.m_LastX = static_cast<float>(event.GetX());
        camera.m_LastY = static_cast<float>(event.GetY());

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.m_CurrentYaw -= xoffset;
        camera.m_CurrentPitch -= yoffset;

        if (camera.m_CurrentPitch > 89.0f)
            camera.m_CurrentPitch = 89.0f;
        if (camera.m_CurrentPitch < -89.0f)
            camera.m_CurrentPitch = -89.0f;

        camera.RecomputeViewMatrix(camera.m_CurrentYaw, camera.m_CurrentPitch);
        return true;
    }

    void Camera::RecomputeViewMatrix(float yaw, float pitch)
    {
        m_CameraFront.x = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        m_CameraFront.y = sin(glm::radians(pitch));
        m_CameraFront.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        m_CameraFront = glm::normalize(m_CameraFront);
    }
} // namespace Core
