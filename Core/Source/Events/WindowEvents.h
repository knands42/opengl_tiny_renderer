#pragma once

#include "Event.h"

namespace Core
{
    class WindowClosedEvent : public Event
    {
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
            return "WindowResizeEvent: " + m_Width + " " + m_Height;
        }

        EVENT_CLASS_TYPE(WindowResize)

    private:
        uint32_t m_Width, m_Height;
    };
} // namespace Core
