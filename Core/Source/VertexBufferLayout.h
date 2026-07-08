#pragma once
#include <ranges>
#include <vector>

#include "GLError.h"

namespace Core
{

    struct VertexBufferElement
    {
        unsigned int type;
        unsigned int count;
        unsigned char normalized;

        static unsigned int GetSizeOfType(const unsigned int type)
        {
            switch (type)
            {
            case GL_FLOAT:
                return 4;
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
            }
            ASSERT(false);
            return 0;
        }
    };

    class VertexBufferLayout
    {
    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;

    public:
        VertexBufferLayout() : m_Stride(0)
        {
        }

        template <typename T> void Push(unsigned int count, bool normalized = GL_FALSE)
        {
            static_assert(sizeof(T) == 0, "Push not supported for this type");
        }

        inline auto GetElements() const -> std::vector<VertexBufferElement>
        {
            return m_Elements;
        }

        inline auto GetStride() const -> unsigned int
        {
            return m_Stride;
        }
    };

    template <> inline void VertexBufferLayout::Push<float>(const unsigned int count, const bool normalized)
    {
        m_Elements.push_back({.type = GL_FLOAT, .count = count, .normalized = normalized});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
    }

    template <> inline void VertexBufferLayout::Push<unsigned int>(const unsigned int count, const bool normalized)
    {
        m_Elements.push_back({.type = GL_UNSIGNED_INT, .count = count, .normalized = normalized});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
    }

    template <> inline void VertexBufferLayout::Push<unsigned char>(const unsigned int count, const bool normalized)
    {
        m_Elements.push_back({.type = GL_UNSIGNED_BYTE, .count = count, .normalized = normalized});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
    }
} // namespace Core
