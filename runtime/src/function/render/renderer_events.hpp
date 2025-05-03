#pragma once

#include "core/event.hpp"
#include "function/render/renderer.hpp"

namespace Arcadia::Events
{
    class RendererBuilt: public EventBase
    {
    public:
        RendererBuilt(std::shared_ptr<RendererInterface> renderer_sptr):
            spRenderer(renderer_sptr)
        {}
    public:
        const std::shared_ptr<RendererInterface> spRenderer;
    };

    class RendererUnbuilt: public EventBase
    {};
}
