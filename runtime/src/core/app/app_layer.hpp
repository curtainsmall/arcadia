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

    virtual void OnEvent(EventBase&) override = 0;
    virtual void OnUpdate() override = 0;
};

ARCADIA_API auto CreateApplication() -> std::unique_ptr<iAppLayer>;
