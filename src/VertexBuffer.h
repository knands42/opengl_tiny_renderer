#pragma once

#include "glad.h"

class VertexBuffer
{
public:
    VertexBuffer() = default;
    ~VertexBuffer();

    void Bind();
    void Unbind() const;

    [[nodiscard]] auto GetVAO() const -> unsigned int { return VAO; }
private:
    unsigned int VBO, VAO, EBO;
};
