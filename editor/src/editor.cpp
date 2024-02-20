#include "editor.hpp"

#include"core/event/event.hpp"
#include"entry_point.hpp"

arcadia::editor::editor()
{
    auto& layer_stack = arcadia::layer_stack::instance();
    layer_stack.push_layer<arcadia::window>();
    _running = true;
}

arcadia::editor::~editor() = default;

void arcadia::editor::on_update(delta_time_type delta_time)
{}

auto arcadia::editor::on_event(const arcadia::event& event) -> bool
{
    ARCADIA_DISPATCH_EVENT(arcadia::window_close, event, _on_window_close);

    return true;
}

auto arcadia::editor::_on_window_close(const arcadia::window_close& event) -> bool
{
    _running = false;
    return true;
}

auto arcadia::create_application_uptr() -> std::unique_ptr<arcadia::application>
{
    return std::make_unique<arcadia::editor>();
}
