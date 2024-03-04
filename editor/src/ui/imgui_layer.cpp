#include "imgui_layer.hpp"

#include"function/input/input_events.hpp"
#include"function/window/monitor.hpp"

#include"ui/backend.hpp"
#include"ui/imgui_windows/imgui_window_manubar.hpp"
#include"ui/imgui_windows/imgui_window_outliner.hpp"
#include"ui/imgui_windows/imgui_window_popup_create_entity.hpp"
#include"ui/imgui_windows/imgui_window_popup_create_project.hpp"
#include"ui/imgui_windows/imgui_window_popup_create_scene.hpp"
#include"ui/imgui_windows/imgui_window_viewport.hpp"


arcadia::imgui_layer::imgui_layer(arcadia::window_layer& window):
    arcadia::layer_interface("imgui"),
    _window_ptr(&window)
{
    _imgui_context_ptr = ImGui::CreateContext();

    auto& io = _imgui_context_ptr->IO;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags =
        ImGuiConfigFlags_DockingEnable
        | ImGuiConfigFlags_NoMouseCursorChange
        | ImGuiConfigFlags_ViewportsEnable;
    arcadia::imgui_backend::initialize(*_window_ptr);

    std::initializer_list<std::string> imgui_window_titles{
        arcadia::imgui_window_outliner::get_title(),
        arcadia::imgui_window_viewport::get_title()
    };
    _emplace_imgui_window<arcadia::imgui_window_menubar>(true, imgui_window_titles);
    _emplace_imgui_window<arcadia::imgui_window_popup_create_project>(false);
    _emplace_imgui_window<arcadia::imgui_window_popup_create_scene>(false);
    _emplace_imgui_window<arcadia::imgui_window_popup_create_entity>(false);
    _emplace_imgui_window<arcadia::imgui_window_outliner>(false);
    _emplace_imgui_window<arcadia::imgui_window_viewport>(false);
}

arcadia::imgui_layer::~imgui_layer()
{
    if(_imgui_context_ptr)
    {
        arcadia::imgui_backend::shutdown(*_window_ptr);
        ImGui::DestroyContext(_imgui_context_ptr);
    }
}

void arcadia::imgui_layer::on_event(arcadia::event_base& event)
{
    arcadia::imgui_backend::imgui_on_event(event);
    for(auto& imgui_window_uptr : _imgui_window_uptrs)
    {
        imgui_window_uptr->on_event(event);
    }
}

void arcadia::imgui_layer::on_update(delta_time_type delta_time)
{
    ImGui::SetCurrentContext(_imgui_context_ptr);

    arcadia::imgui_backend::new_frame(*_window_ptr);
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport();

    if(show_demo_window)
    {
        ImGui::ShowDemoWindow();
    }

    if(show_debug_info)
    {
        ImGui::ShowStackToolWindow();
        ImGui::ShowMetricsWindow();
    }

    for(auto& imgui_window_uptr : _imgui_window_uptrs)
    {
        imgui_window_uptr->on_update();
    }

    ImGui::Render();
    arcadia::imgui_backend::render_draw_data(*_window_ptr);

    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
