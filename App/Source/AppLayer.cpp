#include "AppLayer.h"

#include <cassert>
#include <print>

namespace App
{
    AppLayer::AppLayer()
    {
        std::println("AppLayer");
    }

    AppLayer::~AppLayer()
    {
    }

    void AppLayer::OnEvent(Core::Event& event)
    {
    }

    void AppLayer::OnUpdate(float ts)
    {
    }

    void AppLayer::OnRender()
    {
    }
} // namespace App
