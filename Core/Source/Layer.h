#pragma once

#include <memory>

#include "Events/Event.h"
#include "Window.h"

namespace Core
{

    class Layer
    {
    public:
        virtual ~Layer() = default;

        virtual void OnEvent(Event& event)
        {
        }

        virtual void OnUpdate(float ts)
        {
        }
        virtual void OnRender()
        {
        }

        void SetWindow(Window *window)
        {
            m_Window = window;
        }

        template <std::derived_from<Layer> T, typename... Args> void TransitionTo(Args&&...args)
        {
            QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
        }

    protected:
        Core::Window *m_Window = nullptr;

    private:
        void QueueTransition(std::unique_ptr<Layer> layer);
    };

} // namespace Core
