#pragma once

#include <functional>

namespace Core
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
    };

#define EVENT_CLASS_TYPE(type)                                                                                         \
    static EventType GetStaticType()                                                                                   \
    {                                                                                                                  \
        return EventType::type;                                                                                        \
    }                                                                                                                  \
    virtual EventType GetEventType() const override                                                                    \
    {                                                                                                                  \
        return GetStaticType();                                                                                        \
    }                                                                                                                  \
    virtual const char *GetName() const override                                                                       \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }

    class Event
    {
    public:
        bool Handled = false;

        virtual ~Event() = default;
        virtual EventType GetEventType() const = 0;
        virtual const char *GetName() const = 0;
        virtual const char *ToString() const
        {
            return GetName();
        }
    };

    class EventDispatcher
    {
        template <typename T> using EventFn = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& event) : m_Event(event)
        {
        }

        template <typename T> bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetEventType() == T::GetStaticType() && !m_Event.Handled)
            {
                m_Event.Handled = func(*(T *)&m_Event);
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };
} // namespace Core
