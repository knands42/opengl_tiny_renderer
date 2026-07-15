#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Core
{
    class VertexArray
    {
    private:
        unsigned int m_RendererID;
        unsigned int m_Count;

    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
        unsigned int GetCount() const;
    };
} // namespace Core
