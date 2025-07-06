#pragma once

#include "core/layer.hpp"
#include "core/time.hpp"
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

        void Start();
        void Stop();
        auto IsRunning() const -> bool;
        void NextLoop();


        auto GetDeltaTime() const -> std::chrono::nanoseconds;

    private:
        bool _Running{ false };
        Timer _Timer{};
        std::chrono::nanoseconds _DeltaTime{};
    };
}
