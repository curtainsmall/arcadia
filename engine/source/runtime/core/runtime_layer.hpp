#pragma once

#include "core/layer.hpp"
#include "core/time.hpp"
#include "core/runtime_events.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    struct ACDA_API RuntimeLayer: public LayerInterface
    {
    public:
        RuntimeLayer();
        virtual ~RuntimeLayer() override;

        virtual void OnEvent(EventBase& event) override;
        virtual void OnUpdate() override;

        auto IsRunning() const -> bool;
        auto GetDeltaTime() const -> std::chrono::nanoseconds;

    private:
        void _OnRuntimeStart(Events::RuntimeStart& e);
        void _OnRuntimeStop(Events::RuntimeStop& e);

    private:
        bool _Running{ false };
        Timer _Timer{};
        std::chrono::nanoseconds _DeltaTime{};
    };
}
