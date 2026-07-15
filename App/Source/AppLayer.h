#pragma once

#include <glad/gl.h>

#include "Events/Event.h"
#include "Layer.h"

namespace App
{
    class AppLayer : public Core::Layer
    {
        public:
            AppLayer();
            ~AppLayer();

            virtual void OnEvent(Core::Event& event) override;
            virtual void OnUpdate(float ts) override;

            virtual void OnRender() override;
        private:
            uint32_t m_Shader = 0;
            uint32_t m_VertexArray = 0;
            uint32_t m_VertexBuffer = 0;
    };
} // namespace App
