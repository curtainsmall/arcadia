#pragma once

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"

namespace arcadia
{
    struct ARCADIA_API render_layer: arcadia::layer_interface
    {
    public:
        render_layer() = default;
        virtual ~render_layer()  = default;

        virtual auto on_event(const arcadia::event_base& event) ->
    };
}
