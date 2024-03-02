#include "editor_layer.hpp"

#include<memory>

#include"core/app/app_config.hpp"
#include"core/app/app_context.hpp"
#include"core/entry_point.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"

arcadia::editor_app_layer::editor_app_layer()
{
    auto& layer_stack = arcadia::layer_stack::instance();
    const auto& app_config = arcadia::app_config::instance();
    auto& app_context = arcadia::app_context::instance();

    // Window layer
    {
        _main_window_ptr = &layer_stack
            .push_layer<arcadia::window_layer>(
                app_config.window_size,
                app_config.window_title,
                app_config.window_multisample_count
            )
            .top<arcadia::window_layer>();
    }

    // Project layer
    {
        _project_ptr = &layer_stack
            .push_layer<arcadia::project_layer>()
            .top<arcadia::project_layer>();
    }

    // ImGui layer
    {
        _main_ui_ptr = &layer_stack
            .push_layer<arcadia::imgui_layer>(*_main_window_ptr)
            .top<arcadia::imgui_layer>();
    }
    app_context.running = true;
}

void arcadia::editor_app_layer::on_update(delta_time_type delta_time)
{}

void arcadia::editor_app_layer::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .bind_handler<arcadia::event::window_close>(ARCADIA_BIND_MEMBER_FN(_on_window_close))
        .dispatch();
}

void arcadia::editor_app_layer::_on_window_close(const arcadia::event::window_close& e)
{
    const auto& [wnd_ptr] = e.data_tuple;
    auto& app_config = arcadia::app_config::instance();
    app_config.window_size = wnd_ptr->get_size();
    app_config.window_pos = wnd_ptr->get_pos();
    app_config.window_maxmized = wnd_ptr->get_size_state() == arcadia::window_size_state::maxmized;

    auto& app_context = arcadia::app_context::instance();
    app_context.running = false;
}

auto arcadia::create_application_uptr() -> std::unique_ptr<arcadia::app_layer>
{
    return std::make_unique<arcadia::editor_app_layer>();
}
