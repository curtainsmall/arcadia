#include "editor_layer.hpp"

#include<memory>

#include"core/app/app_config.hpp"
#include"core/app/app_context.hpp"
#include"core/entry_point.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_windows/imgui_window_manubar.hpp"
#include"ui/imgui_windows/imgui_window_outliner.hpp"
#include"ui/imgui_windows/imgui_window_property.hpp"
#include"ui/imgui_windows/imgui_window_state.hpp"
#include"ui/imgui_windows/imgui_window_viewport.hpp"

arcadia::editor_app_layer::editor_app_layer()
{
    auto& layer_stack = arcadia::layer_stack::instance();
    const auto& app_config = arcadia::app_config::instance();
    auto& app_context = arcadia::app_context::instance();
    auto& editor_context = arcadia::editor_context::instance();

    // Window layer
    {
        editor_context.main_window_layer_wptr = layer_stack
            .push_layer<arcadia::window_layer>(
                app_config.window_size,
                app_config.window_title,
                app_config.window_multisample_count
            )
            .top<arcadia::window_layer>();
    }

    // Project layer
    {
        editor_context.main_project_layer_wptr = layer_stack
            .push_layer<arcadia::project_layer>()
            .top<arcadia::project_layer>();
    }

    // Editor ImGui layer
    {
        editor_context.main_imgui_layer_wptr = layer_stack
            .push_layer<arcadia::imgui_layer>(
                editor_context.main_window_layer_wptr.lock(),
                ARCADIA_BIND_MEMBER_FN(_imgui_window_installer),
                arcadia::imgui_style_dark
            )
            .top<arcadia::imgui_layer>();

        //editor_context.main_imgui_layer_wptr.lock()->show_demo_window = true;
    }
    app_context.running = true;
}

void arcadia::editor_app_layer::on_update()
{}

void arcadia::editor_app_layer::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::window_should_close>(ARCADIA_BIND_MEMBER_FN(_on_window_should_close))
        .dispatch<arcadia::event::project_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_project_unbuilt))
        .result();
}

void arcadia::editor_app_layer::_imgui_window_installer(arcadia::imgui_layer& imgui_layer)
{
    const auto& app_config = arcadia::app_config::instance();
    const auto& id_strs = app_config.imgui_opened_window_id_strs;

    std::initializer_list<std::tuple<std::string, std::string>> imgui_window_ids{
        std::make_tuple("Outliner"s,arcadia::imgui_window_outliner::get_id_str_static()),
        std::make_tuple("Viewport"s,arcadia::imgui_window_viewport::get_id_str_static()),
        std::make_tuple("Property"s,arcadia::imgui_window_property::get_id_str_static()),
        std::make_tuple("State"s,arcadia::imgui_window_state::get_id_str_static())
    };
    imgui_layer
        .emplace_imgui_window<arcadia::imgui_window_menubar>(true, imgui_window_ids)
        .emplace_imgui_window<arcadia::imgui_window_outliner>(id_strs.contains(arcadia::imgui_window_outliner::get_id_str_static()), "Outliner")
        .emplace_imgui_window<arcadia::imgui_window_viewport>(id_strs.contains(arcadia::imgui_window_viewport::get_id_str_static()), "Viewport")
        .emplace_imgui_window<arcadia::imgui_window_property>(id_strs.contains(arcadia::imgui_window_property::get_id_str_static()), "Property")
        .emplace_imgui_window<arcadia::imgui_window_state>(id_strs.contains(arcadia::imgui_window_state::get_id_str_static()), "State");
}

void arcadia::editor_app_layer::_stop()
{
    auto& editor_context = arcadia::editor_context::instance();
    auto main_window_layer_sptr = editor_context.main_window_layer_wptr.lock();
    auto main_imgui_layer_sptr = editor_context.main_imgui_layer_wptr.lock();

    auto& app_config = arcadia::app_config::instance();
    app_config.window_size = main_window_layer_sptr->get_size();
    app_config.window_pos = main_window_layer_sptr->get_pos();
    app_config.window_maxmized = main_window_layer_sptr->get_size_state() == arcadia::window_size_state::maxmized;

    for(const auto& imgui_window_uptr : main_imgui_layer_sptr->get_imgui_window_uptrs())
    {
        if(imgui_window_uptr->is_open())
        {
            app_config.imgui_opened_window_id_strs.emplace(imgui_window_uptr->get_id_str());
        }
    }

    auto& app_context = arcadia::app_context::instance();
    app_context.running = false;
}

void arcadia::editor_app_layer::_on_window_should_close(arcadia::event::window_should_close& e)
{
    auto& editor_context = arcadia::editor_context::instance();
    auto main_window_layer_sptr = editor_context.main_window_layer_wptr.lock();
    auto main_project_layer_sptr = editor_context.main_project_layer_wptr.lock();

    const auto& [wnd_ptr] = e.data_tuple;
    if(wnd_ptr == main_window_layer_sptr.get() && main_project_layer_sptr->has_project())
    {
        _waiting_for_project_unbuilt_before_closing = true;
    }
    else
    {
        _stop();
    }
}

void arcadia::editor_app_layer::_on_project_unbuilt(arcadia::event::project_unbuilt& e)
{
    if(_waiting_for_project_unbuilt_before_closing)
    {
        _stop();
    }
}

void arcadia::editor_app_layer::_on_window_close_canceled(arcadia::event::window_close_canceled& e)
{
    _waiting_for_project_unbuilt_before_closing = false;
}

auto arcadia::create_application_uptr() -> std::unique_ptr<arcadia::app_layer>
{
    return std::make_unique<arcadia::editor_app_layer>();
}
