#include "VertexBuffer.h"

#include <vector>

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, const unsigned int size) : m_Size(size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    Unbind();
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

auto VertexBuffer::Bind() const -> void
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

auto VertexBuffer::Unbind() const -> void
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0))
}

auto VertexBuffer::EnablePolygonMode() const -> void
{
    // wireframe polygons
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}