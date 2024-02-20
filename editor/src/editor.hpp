#pragma once

#include<memory>

#include"core/application/application.hpp"
#include"function/window/window.hpp"

namespace arcadia
{
    struct editor: arcadia::application
    {
    public:
        editor();
        virtual ~editor();

        virtual auto on_event(const arcadia::event& event) -> bool override;
        virtual void on_update(delta_time_type delta_time) override;


    private:
        auto _on_window_close(const arcadia::window_close& event) -> bool;

    private:
        arcadia::window* _main_window_ptr{ nullptr };
    };

    ARCADIA_API auto create_application_uptr() -> std::unique_ptr<arcadia::application>;
}
