#pragma once

#include <cstdio>

#include "Event.h"

namespace Core
{

    //
    // Key Events
    //

    class KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const
        {
            return m_KeyCode;
        }

    protected:
        KeyEvent(int keycode) : m_KeyCode(keycode)
        {
        }

        int m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keycode, bool isRepeat) : KeyEvent(keycode), m_IsRepeat(isRepeat)
        {
        }

        inline bool IsRepeat() const
        {
            return m_IsRepeat;
        }

        const char *ToString() const override
        {
            snprintf(m_StringBuffer, sizeof(m_StringBuffer), "KeyPressedEvent: %d (repeat=%d)", m_KeyCode, m_IsRepeat);
            return m_StringBuffer;
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        bool m_IsRepeat;
        mutable char m_StringBuffer[64];
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode) : KeyEvent(keycode)
        {
        }

        const char *ToString() const override
        {
            snprintf(m_StringBuffer, sizeof(m_StringBuffer), "KeyReleasedEvent: %d", m_KeyCode);
            return m_StringBuffer;
        }

        EVENT_CLASS_TYPE(KeyReleased)
    private:
        mutable char m_StringBuffer[64];
    };

    //
    // Mouse Events
    //

    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(double x, double y) : m_MouseX(x), m_MouseY(y)
        {
        }

        inline double GetX() const
        {
            return m_MouseX;
        }
        inline double GetY() const
        {
            return m_MouseY;
        }

        const char *ToString() const override
        {
            snprintf(m_StringBuffer, sizeof(m_StringBuffer), "MouseMovedEvent: %.2f, %.2f", m_MouseX, m_MouseY);
            return m_StringBuffer;
        }

        EVENT_CLASS_TYPE(MouseMoved)
    private:
        double m_MouseX, m_MouseY;
        mutable char m_StringBuffer[64];
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(double xOffset, double yOffset) : m_XOffset(xOffset), m_YOffset(yOffset)
        {
        }

        inline double GetXOffset() const
        {
            return m_XOffset;
        }
        inline double GetYOffset() const
        {
            return m_YOffset;
        }

        const char *ToString() const override
        {
            snprintf(m_StringBuffer, sizeof(m_StringBuffer), "MouseScrolledEvent: %.2f, %.2f", m_XOffset, m_YOffset);
            return m_StringBuffer;
        }

        EVENT_CLASS_TYPE(MouseScrolled)
    private:
        double m_XOffset, m_YOffset;
        mutable char m_StringBuffer[64];
    };

    class MouseButtonEvent : public Event
    {
    public:
        inline int GetMouseButton() const
        {
            return m_Button;
        }

    protected:
        MouseButtonEvent(int button) : m_Button(button)
        {
        }

        int m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button)
        {
        }

        const char *ToString() const override
        {
            snprintf(m_StringBuffer, sizeof(m_StringBuffer), "MouseButtonPressedEvent: %d", m_Button);
            return m_StringBuffer;
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    private:
        mutable char m_StringBuffer[64];
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button)
        {
        }

        const char *ToString() const override
        {
            snprintf(m_StringBuffer, sizeof(m_StringBuffer), "MouseButtonReleasedEvent: %d", m_Button);
            return m_StringBuffer;
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    private:
        mutable char m_StringBuffer[64];
    };

} // namespace Core
