#include"pch.hpp"
#include "imgui_layer.hpp"

#include"function/input/input_events.hpp"
#include"function/ui/imgui_backend.hpp"

arcadia::imgui_layer::imgui_layer(
    const std::shared_ptr<const arcadia::window_layer>& window_layer_sptr,
    const std::function<void(arcadia::imgui_layer&)>& imgui_window_installer,
    const std::function<void()>& imgui_style_setter
):
    arcadia::layer_interface("imgui"),
    _window_wptr(window_layer_sptr)
{
    _imgui_context_ptr = ImGui::CreateContext();
    ImGui::SetCurrentContext(_imgui_context_ptr);

    auto& io = _imgui_context_ptr->IO;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags =
        ImGuiConfigFlags_DockingEnable
        | ImGuiConfigFlags_NoMouseCursorChange
        | ImGuiConfigFlags_ViewportsEnable;
    arcadia::imgui_backend::initialize(*_window_wptr.lock());

    imgui_style_setter();

    imgui_window_installer(*this);
}

arcadia::imgui_layer::~imgui_layer()
{
    if(_imgui_context_ptr)
    {
        arcadia::imgui_backend::shutdown(*_window_wptr.lock());
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

void arcadia::imgui_layer::on_update()
{
    auto window_sptr = _window_wptr.lock();

    ImGui::SetCurrentContext(_imgui_context_ptr);

    arcadia::imgui_backend::new_frame(*window_sptr);
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport();

    if(show_debug_info)
    {
        ImGui::ShowStackToolWindow();
        ImGui::ShowMetricsWindow();
    }

    if(show_demo_window)
    {
        ImGui::ShowDemoWindow();
    }
    else
    {

        for(auto& imgui_window_uptr : _imgui_window_uptrs)
        {
            imgui_window_uptr->on_update();
        }
    }

    ImGui::Render();
    arcadia::imgui_backend::render_draw_data(*window_sptr);

    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
