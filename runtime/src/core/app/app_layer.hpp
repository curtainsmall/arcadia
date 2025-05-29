#pragma once

#include <memory>

#include "core/event.hpp"
#include "core/layer.hpp"
#include "platform/api_def.hpp"

auto main() -> int;

namespace Arcadia
{
    struct ACDA_API AppLayerInterface: public LayerInterface
    {
        friend auto ::main() -> int;
    public:
        using SelfType = AppLayerInterface;
    public:
        AppLayerInterface();
        virtual ~AppLayerInterface();

        virtual void OnEvent(EventBase&) override = 0;
        virtual void OnUpdate() override = 0;
    };

    ACDA_API auto CreateApplicationUnique() -> std::unique_ptr<AppLayerInterface>;
}