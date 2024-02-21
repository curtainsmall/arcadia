#include "editor.hpp"

#include<memory>

#include"core/event/event.hpp"
#include"core/layer/layer.hpp"
#include"entry_point.hpp"
#include"function/input/input.hpp"

arcadia::editor_app_layer::editor_app_layer()
{
    auto& layer_stack = arcadia::layer_stack::instance();

    // Window layer
    {
        auto window_layer_uptr = std::make_unique<arcadia::window_layer>();
        _main_window_ptr = window_layer_uptr.get();
        layer_stack.push_layer<arcadia::window_layer>(std::move(window_layer_uptr));
    }

    /// ImGui layer
    {
        auto imgui_layer_uptr = std::make_unique<arcadia::imgui_layer>(*_main_window_ptr);
        _main_ui_ptr = imgui_layer_uptr.get();
        layer_stack.push_layer<arcadia::imgui_layer>(std::move(imgui_layer_uptr));
    }
    _running = true;
}

arcadia::editor_app_layer::~editor_app_layer() = default;

void arcadia::editor_app_layer::on_update(delta_time_type delta_time)
{}

auto arcadia::editor_app_layer::on_event(const arcadia::event& event) -> bool
{
    ARCADIA_DISPATCH_EVENT(arcadia::window_close, event, _on_window_close);
    auto debug_cursor_pos_info = [&](const arcadia::input_cursor_pos& cursor_pos) -> bool
    {
        const auto& [wnd_ptr, pos] = cursor_pos.data_tuple;
        _main_window_ptr->set_title(std::format("x:{},y:{}", pos.x, pos.y));
        return false;
    };
    ARCADIA_DISPATCH_EVENT(arcadia::input_cursor_pos, event, debug_cursor_pos_info
    );

    return false;
}

auto arcadia::editor_app_layer::_on_window_close(const arcadia::window_close& event) -> bool
{
    _running = false;
    return false;
}

auto arcadia::create_application_uptr() -> std::unique_ptr<arcadia::app_layer>
{
    return std::make_unique<arcadia::editor_app_layer>();
}
