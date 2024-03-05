#include "editor_layer.hpp"

#include<memory>

#include"core/app/app_config.hpp"
#include"core/app/app_context.hpp"
#include"core/entry_point.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"

#include"ui/imgui_windows/imgui_window_manubar.hpp"
#include"ui/imgui_windows/imgui_window_outliner.hpp"
#include"ui/imgui_windows/imgui_window_popup_create_entity.hpp"
#include"ui/imgui_windows/imgui_window_popup_create_project.hpp"
#include"ui/imgui_windows/imgui_window_popup_create_scene.hpp"
#include"ui/imgui_windows/imgui_window_property.hpp"
#include"ui/imgui_windows/imgui_window_viewport.hpp"

arcadia::editor_app_layer::editor_app_layer()
{
    auto& layer_stack = arcadia::layer_stack::instance();
    const auto& app_config = arcadia::app_config::instance();
    auto& app_context = arcadia::app_context::instance();

    // Window layer
    {
        _editor_window_ptr = &layer_stack
            .push_layer<arcadia::window_layer>(
                app_config.window_size,
                app_config.window_title,
                app_config.window_multisample_count
            )
            .top<arcadia::window_layer>();
    }

    // Project layer
    {
        _project_cptr = &layer_stack
            .push_layer<arcadia::project_layer>()
            .top<arcadia::project_layer>();
    }

    // Editor ImGui layer
    {
        _editor_ui_ptr = &layer_stack
            .push_layer<arcadia::imgui_layer>(*_editor_window_ptr, ARCADIA_BIND_MEMBER_FN(_imgui_window_installer))
            .top<arcadia::imgui_layer>();
    }
    app_context.running = true;
}

void arcadia::editor_app_layer::on_update(delta_time_type delta_time)
{}

void arcadia::editor_app_layer::_imgui_window_installer(arcadia::imgui_layer& imgui_layer)
{
    const auto& app_config = arcadia::app_config::instance();
    const auto& id_strs = app_config.imgui_opened_window_id_strs;

    std::initializer_list<std::tuple<std::string, std::string>> imgui_window_ids{
        std::make_tuple("Outliner"s,arcadia::imgui_window_outliner::get_id_str_static()),
        std::make_tuple("Viewport"s,arcadia::imgui_window_viewport::get_id_str_static()),
        std::make_tuple("Property"s,arcadia::imgui_window_property::get_id_str_static()),
    };
    imgui_layer
        .emplace_imgui_window<arcadia::imgui_window_menubar>(true, imgui_window_ids)
        .emplace_imgui_window<arcadia::imgui_window_popup_create_project>(false, "Create Project")
        .emplace_imgui_window<arcadia::imgui_window_popup_create_scene>(false, "Create Scene")
        .emplace_imgui_window<arcadia::imgui_window_popup_create_entity>(false, "Create Entity")
        .emplace_imgui_window<arcadia::imgui_window_outliner>(id_strs.contains(arcadia::imgui_window_outliner::get_id_str_static()), "Outliner")
        .emplace_imgui_window<arcadia::imgui_window_viewport>(id_strs.contains(arcadia::imgui_window_viewport::get_id_str_static()), "Viewport")
        .emplace_imgui_window<arcadia::imgui_window_property>(id_strs.contains(arcadia::imgui_window_property::get_id_str_static()), "Property");
}

void arcadia::editor_app_layer::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
    .dispatch<arcadia::event::window_close>(ARCADIA_BIND_MEMBER_FN(_on_window_close));
}

void arcadia::editor_app_layer::_on_window_close(const arcadia::event::window_close& e)
{
    const auto& [wnd_ptr] = e.data_tuple;
    if(wnd_ptr == _editor_window_ptr)
    {
        auto& app_config = arcadia::app_config::instance();
        app_config.window_size = wnd_ptr->get_size();
        app_config.window_pos = wnd_ptr->get_pos();
        app_config.window_maxmized = wnd_ptr->get_size_state() == arcadia::window_size_state::maxmized;

        for(const auto& imgui_window_ptr : _editor_ui_ptr->get_imgui_window_uptrs())
        {
            if(imgui_window_ptr->is_open())
            {
                app_config.imgui_opened_window_id_strs.emplace(imgui_window_ptr->get_id_str());
            }
        }

        auto& app_context = arcadia::app_context::instance();
        app_context.running = false;
    }
}

auto arcadia::create_application_uptr() -> std::unique_ptr<arcadia::app_layer>
{
    return std::make_unique<arcadia::editor_app_layer>();
}
