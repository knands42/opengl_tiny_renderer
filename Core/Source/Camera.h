#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>

namespace Core
{

    class Camera
    {
    private:
        float m_AspectRatio;  // TODO: how to use this?
        glm::vec3 m_Position; // TODO: when to update?

    public:
        Camera();
        ~Camera();

        [[nodiscard]] glm::mat4 GetViewMatrix(glm::vec3 at) const;
        [[nodiscard]] glm::mat4 GetViewMatrix(float yaw, float pitch) const;
        [[nodiscard]] glm::mat4 GetProjectionMatrix(float fov, float width, float height, float nearPlane,
                                                    float farPlane) const;
        [[nodiscard]] glm::mat4 GetModelMatrix() const;
        void SetPosition(glm::vec3 position);
    };
} // namespace Core
