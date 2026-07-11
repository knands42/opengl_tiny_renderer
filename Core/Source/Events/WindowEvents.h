#pragma once

#include <cstdio>
#include "Event.h"
#include <glad/gl.h>

namespace Core
{
    class WindowClosedEvent : public Event
    {
    public:
        WindowClosedEvent()
        {
        }

        EVENT_CLASS_TYPE(WindowClose)
    };

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
        {
        }

        inline uint32_t GetWidth() const
        {
            return m_Width;
        }
        inline uint32_t GetHeight() const
        {
            return m_Height;
        }

        const char *ToString() const override
        {
            snprintf(m_StringBuffer, sizeof(m_StringBuffer), "WindowResizeEvent: %.2d %.2d", m_Width, m_Height);
            return m_StringBuffer;
        }

        EVENT_CLASS_TYPE(WindowResize)

    private:
        uint32_t m_Width, m_Height;
        mutable char m_StringBuffer[64];
    };
} // namespace Core
