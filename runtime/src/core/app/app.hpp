#pragma once

#include<filesystem>
#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"
#include"core/time/timer.hpp"

auto main(
    int argc,
    const char** argv
) -> int;

namespace arcadia
{
    struct ARCADIA_API app_layer: arcadia::layer_interface
    {
        friend auto ::main(int, const char**) -> int;
    public:
        using self_type = arcadia::app_layer;
    public:
        app_layer();
        virtual ~app_layer() = default;


        virtual inline auto on_event(const arcadia::event&) -> bool override = 0;
        virtual inline void on_update(delta_time_type delta_time) override = 0;

    private:
        void run();

    #ifndef NDEBUG
        void _debug_log_event(const arcadia::event& event);
    #endif

    public:
        static inline std::filesystem::path working_directory{ "./" };

    protected:
        bool _running{ true };
        arcadia::timer _timer{};
    };

    ARCADIA_API auto create_application_uptr() -> std::unique_ptr<arcadia::app_layer>;
}
