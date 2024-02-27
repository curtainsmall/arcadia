#include "editor.hpp"

#include<memory>

#include"core/app/app_config.hpp"
#include"core/app/app_context.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"
#include"entry_point.hpp"

arcadia::editor_app_layer::editor_app_layer()
{
    auto& layer_stack = arcadia::layer_stack::instance();
    const auto& app_config = arcadia::app_config::instance();
    auto& app_context = arcadia::app_context::instance();

    // Window layer
    {
        layer_stack.push_layer<arcadia::window_layer>();
        _main_window_ptr = &layer_stack.top<arcadia::window_layer>();
    }

    // Project layer
    {
        layer_stack.push_layer<arcadia::project_layer>();
        _project_ptr = &layer_stack.top<arcadia::project_layer>();
    }

    // ImGui layer
    {
        layer_stack.push_layer<arcadia::imgui_layer>(*_main_window_ptr);
        _main_ui_ptr = &layer_stack.top<arcadia::imgui_layer>();
    }
    app_context.running = true;
}

void arcadia::editor_app_layer::on_update(delta_time_type delta_time)
{}

auto arcadia::editor_app_layer::on_event(const arcadia::event_base& event) -> bool
{
    ARCADIA_DISPATCH_EVENT(arcadia::event::window_close, event, _on_window_close);
    return false;
}

auto arcadia::editor_app_layer::_on_window_close(const arcadia::event::window_close& event) -> bool
{
    auto& app_context = arcadia::app_context::instance();
    app_context.running = false;
    return false;
}

auto arcadia::create_application_uptr() -> std::unique_ptr<arcadia::app_layer>
{
    return std::make_unique<arcadia::editor_app_layer>();
}
