#include "Camera.h"

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

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);
    }

    glm::mat4 Camera::GetProjectionMatrix(const float fov, const float width, const float height, const float nearPlane,
                                          const float farPlane) const
    {
        return glm::perspective(fov, width / height, nearPlane, farPlane);
    }

    glm::mat4 Camera::GetModelMatrix(ModelMatrix& modelMatrix) const
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
        dispatcher.Dispatch<KeyPressedEvent>(this { return HandleKeyPressedEvent(*this, e); });
        dispatcher.Dispatch<MouseMovedEvent>(this { return HandleMouseMovedEvent(*this, e); });
    }

    bool Camera::HandleKeyPressedEvent(Camera& camera, KeyPressedEvent& event)
    {
        float currentFrame = glfwGetTime();
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

    bool Camera::HandleMouseMovedEvent(Camera& camera, MouseMovedEvent& event)
    {
        float xoffset = event.GetX() - camera.m_LastX;
        float yoffset = event.GetY() - camera.m_LastY;
        camera.m_LastX = event.GetX();
        camera.m_LastY = event.GetY();

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.m_CurrentYaw += xoffset;
        camera.m_CurrentPitch += yoffset;

        if (camera.m_CurrentPitch > 89.0f)
            camera.m_CurrentPitch = 89.0f;
        if (camera.m_CurrentPitch < -89.0f)
            camera.m_CurrentPitch = -89.0f;

        camera.RecomputeViewMatrix(camera.m_CurrentPitch, camera.m_CurrentPitch);
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
