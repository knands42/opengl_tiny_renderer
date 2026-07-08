#include "VertexArray.h"

namespace Core
{

    VertexArray::VertexArray() : m_Count(0)
    {
        GLCall(glGenVertexArrays(1, &m_RendererID));
    }

    VertexArray::~VertexArray()
    {
        GLCall(glDeleteVertexArrays(1, &m_RendererID));
    }

    void VertexArray::Bind() const
    {
        GLCall(glBindVertexArray(m_RendererID));
    }

    void VertexArray::Unbind() const
    {
        GLCall(glBindVertexArray(0));
    }

    void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
    {
        vb.Bind();
        Bind();
        const auto& elements = layout.GetElements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++)
        {
            const auto& element = elements[i];
            GLCall(glEnableVertexAttribArray(i)) GLCall(glVertexAttribPointer(
                i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
            offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
        }

        m_Count = vb.GetSize() / layout.GetStride();
    }

    unsigned int VertexArray::GetCount() const
    {
        return m_Count;
    }
} // namespace Core
