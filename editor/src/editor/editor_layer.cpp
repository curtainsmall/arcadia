#include "editor_layer.hpp"

#include<memory>

#include"core/app/app_config.hpp"
#include"core/app/app_context.hpp"
#include"core/main.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_windows/imgui_window_main_manubar.hpp"
#include"ui/imgui_windows/imgui_window_main_toolbar.hpp"
#include"ui/imgui_windows/imgui_window_outliner.hpp"
#include"ui/imgui_windows/imgui_window_property.hpp"
#include"ui/imgui_windows/imgui_window_state.hpp"
#include"ui/imgui_windows/imgui_window_viewport.hpp"

EditorAppLayer::EditorAppLayer()
{
    auto& layer_stack = LayerStack::instance();
    const auto& app_config = AppConfig::instance();
    auto& app_context = AppContext::instance();
    auto& editor_context = EditorContext::instance();

    // Window layer
    {
        editor_context.main_window_layer = layer_stack
            .push_layer<WindowLayer>(
                app_config.window_size,
                app_config.window_title,
                app_config.window_multisample_count
            )
            .top<WindowLayer>();
    }

    // Project layer
    {
        editor_context.main_project_layer = layer_stack
            .push_layer<ProjectLayer>()
            .top<ProjectLayer>();
    }

    // Editor ImGui layer
    {
        editor_context.main_imgui_layer = layer_stack
            .push_layer<ImguiLayer>(
                editor_context.main_window_layer.lock(),
                ACDA_BIND_MEMBER_FN(_imgui_window_installer),
                imgui_style::dark
            )
            .top<ImguiLayer>();

        //editor_context._wpmain_imgui_layer.lock()->ShowDemoWindow = true;
    }
    app_context.running = true;
}

void EditorAppLayer::on_update()
{}

void EditorAppLayer::on_event(EventBase& e)
{
    EventDispatcher{ e }
        .dispatch<events::WindowShouldClose>(ACDA_BIND_MEMBER_FN(_on_window_should_close))
        .dispatch<events::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_on_project_unbuilt))
        .dispatch<events::PlayMode>(ACDA_BIND_MEMBER_FN(_on_play_mode))
        .dispatch<events::InputKey>(ACDA_BIND_MEMBER_FN(_on_input_key))
        .is_dispatched();
}

void EditorAppLayer::_imgui_window_installer(ImguiLayer& imgui_layer)
{
    const auto& app_config = AppConfig::instance();
    const auto& id_strs = app_config.imgui_opened_window_id_strs;

    std::initializer_list<std::tuple<std::string, std::string>> imgui_window_ids{
        std::make_tuple("Outliner"s,ImguiWindowOutliner::get_id_str_static()),
        std::make_tuple("Viewport"s,ImguiWindowViewport::get_id_str_static()),
        std::make_tuple("Property"s,ImguiWindowProperty::get_id_str_static()),
        std::make_tuple("State"s,ImguiWindowState::get_id_str_static())
    };
    imgui_layer
        .emplace_imgui_window<ImguiWindowMainMenubar>(imgui_window_ids)
        .emplace_imgui_window<ImguiWindowMainToolbar>()
        .emplace_imgui_window<ImguiWindowOutliner>(id_strs.contains(ImguiWindowOutliner::get_id_str_static()), "Outliner")
        .emplace_imgui_window<ImguiWindowViewport>(id_strs.contains(ImguiWindowViewport::get_id_str_static()), "Viewport")
        .emplace_imgui_window<ImguiWindowProperty>(id_strs.contains(ImguiWindowProperty::get_id_str_static()), "Property")
        .emplace_imgui_window<ImguiWindowState>(id_strs.contains(ImguiWindowState::get_id_str_static()), "State");
}

void EditorAppLayer::_stop()
{
    auto& editor_context = EditorContext::instance();
    auto main_window_layer = editor_context.main_window_layer.lock();
    auto main_imgui_layer = editor_context.main_imgui_layer.lock();

    auto& app_config = AppConfig::instance();
    app_config.window_size = main_window_layer->size();
    app_config.window_pos = main_window_layer->pos();
    app_config.window_maxmized = main_window_layer->size_state() == WindowSizeState::Maxmized;

    for(const auto& imgui_window : main_imgui_layer->get_imgui_window())
    {
        if(imgui_window->open())
        {
            app_config.imgui_opened_window_id_strs.emplace(imgui_window->get_id_str());
        }
    }

    auto& app_context = AppContext::instance();
    app_context.running = false;
}

void EditorAppLayer::_on_window_should_close(events::WindowShouldClose& e)
{
    auto& editor_context = EditorContext::instance();
    auto main_window_layer = editor_context.main_window_layer.lock();
    auto main_project_layer = editor_context.main_project_layer.lock();

    const auto& [p_wnd] = e.data_tuple;
    if(p_wnd == main_window_layer.get() && main_project_layer->has_project())
    {
        _waiting_for_project_unbuilt_before_closing = true;
    }
    else
    {
        _stop();
    }
}

void EditorAppLayer::_on_project_unbuilt(events::ProjectUnbuilt& e)
{
    if(_waiting_for_project_unbuilt_before_closing)
    {
        _stop();
    }
}

void EditorAppLayer::_on_window_close_canceled(events::WindowCloseCanceled& e)
{
    _waiting_for_project_unbuilt_before_closing = false;
}

void EditorAppLayer::_on_play_mode(events::PlayMode& e)
{
    const auto& [state] = e.data_tuple;
    EditorContext::instance().in_play_mode = state;
}

void EditorAppLayer::_on_input_key(events::InputKey& e)
{
    const auto& [wnd, key, scancode, action, mods] = e.data_tuple;

    if(key == InputKey::Escape && to_bool(mods & InputModifier::Shift))
    {
        EditorContext::instance().in_play_mode = false;
    }
}

auto create_application() -> std::unique_ptr<iAppLayer>
{
    return std::make_unique<EditorAppLayer>();
}
