#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/trigonometric.hpp>

#include "Events/InputEvents.h"

namespace Core
{
    Camera::Camera() : m_Position(glm::vec3(0.0f))
    {
    }

    Camera::~Camera()
    {
    }

    glm::mat4 Camera::GetViewMatrix(const glm::vec3 at) const
    {
        return glm::lookAt(m_Position, at, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 Camera::GetViewMatrix(const float yaw, const float pitch) const
    {
        glm::vec3 forward;
        forward.x = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward.y = sin(glm::radians(pitch));
        forward.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward = glm::normalize(forward);
        return glm::lookAt(m_Position, m_Position + forward, glm::vec3(0.0f, 1.0f, 0.0f));
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
        // transform = glm::rotate(transform, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));
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
        dispatcher.Dispatch<KeyPressedEvent>(
            [this](KeyPressedEvent& event)
            {
                float currentFrame = glfwGetTime();
                float deltaTime = currentFrame - m_LastFrame;
                m_LastFrame = currentFrame;
                
                const float cameraSpeed = 0.05f;
                const glm::vec3 cameraFrontTranslation = m_CameraFront * cameraSpeed;
                const glm::vec3 cameraRightTranslation =
                    glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
                
                switch (event.GetKeyCode())
                {
                case GLFW_KEY_W:
                    m_Position += cameraFrontTranslation;
                    break;
                case GLFW_KEY_S:
                    m_Position -= cameraFrontTranslation;
                    break;
                case GLFW_KEY_A:
                    m_Position += cameraRightTranslation;
                    break;
                case GLFW_KEY_D:
                    m_Position -= cameraRightTranslation;
                    break;
                default:
                    return false;
                }

                return true;
            });
    }
} // namespace Core
