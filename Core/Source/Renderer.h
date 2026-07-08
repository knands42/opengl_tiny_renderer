#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

namespace Core
{
    class Renderer
    {
    public:
        void Clear() const;

        void Draw(const VertexArray& vertexArray, const Shader& shader, const Texture& texture) const;

        void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader,
                  const Texture& texture) const;
    };
} // namespace Core
