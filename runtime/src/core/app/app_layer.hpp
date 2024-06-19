#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"

auto main() -> int;

struct iAppLayer: iLayer
{
    friend auto ::main() -> int;
public:
    using self_type = iAppLayer;
public:
    iAppLayer();
    virtual ~iAppLayer();

    virtual void on_event(EventBase&) override = 0;
    virtual void on_update() override = 0;
};

ACDA_API auto create_application() -> std::unique_ptr<iAppLayer>;
