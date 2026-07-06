#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "GLError.h"
#include "Texture.h"

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& vertexArray, const Shader& shader, const Texture &texture) const;
    void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader, const Texture &texture) const;
};
