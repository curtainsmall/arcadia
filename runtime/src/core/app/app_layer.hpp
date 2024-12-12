#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"

auto main() -> int;

class iAppLayer: public iLayer
{
    friend auto ::main() -> int;
public:
    using SelfType = iAppLayer;
public:
    iAppLayer();
    virtual ~iAppLayer();

    virtual void OnEvent(EventBase&) override = 0;
    virtual void OnUpdate() override = 0;
};

ACDA_API auto CreateApplication() -> std::unique_ptr<iAppLayer>;