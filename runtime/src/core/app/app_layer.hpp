#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"

auto main(int argc, const char** argv) -> int;

#define ARCADIA ::arcadia::
#define STD ::std::

namespace arcadia
{
    struct ARCADIA_API app_layer: arcadia::layer_interface
    {
        friend auto ::main(int, const char**) -> int;
    public:
        using self_type = arcadia::app_layer;
    public:
        app_layer();
        virtual ~app_layer();

        virtual void on_event(arcadia::event_base&) override = 0;
        virtual void on_update(delta_time_type delta_time) override = 0;
    };

    ARCADIA_API auto create_application_uptr() -> std::unique_ptr<arcadia::app_layer>;
}
