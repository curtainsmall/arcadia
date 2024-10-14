#include"pch.hpp"

#include "imgui_layer.hpp"

#include"function/input/input_events.hpp"
#include"resource/fonts/icon_header.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_backend.hpp"

ImguiLayer::ImguiLayer(
    const std::shared_ptr<const WindowLayer>& window_layer,
    const std::function<void(ImguiLayer&)>& imgui_window_installer,
    const std::function<void()>& imgui_style_setter
) :
    iLayer("imgui"),
    _window(window_layer)
{
    _imgui_context = ImGui::CreateContext();
    ImGui::SetCurrentContext(_imgui_context);

    scale_ui(EditorContext::instance().ui_scale);

    auto& io = _imgui_context->IO;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags =
        ImGuiConfigFlags_DockingEnable
        | ImGuiConfigFlags_NoMouseCursorChange
        | ImGuiConfigFlags_ViewportsEnable;
    io.Fonts->AddFontDefault();

    ImFontConfig imgui_font_config{};
    imgui_font_config.MergeMode = true;
    static const std::array<ImWchar, 3> imgui_icon_ranges{ ICON_MIN_FA, ICON_MAX_FA,0 };
    io.Fonts->AddFontFromFileTTF(font_filepath_str.c_str(), font_size, &imgui_font_config, imgui_icon_ranges.data());

    imgui_backend::initialize(*_window.lock());

    imgui_style_setter();
    imgui_window_installer(*this);
}

ImguiLayer::~ImguiLayer()
{
    if(_imgui_context)
    {
        imgui_backend::shutdown(*_window.lock());
        ImGui::DestroyContext(_imgui_context);
    }
}

void ImguiLayer::on_event(EventBase& e)
{
    // We do not dispatch events to ImGui when the editor is in play mode
    if(EditorContext::instance().in_play_mode)
    {
        return;
    }

    EventDispatcher{ e }
        .dispatch<events::ScaleImguiWindow>(ACDA_BIND_MEMBER_FN(_on_scale_imgui_window))
        .is_dispatched();

    imgui_backend::on_event(e);
    for(auto& imgui_window : _imgui_window)
    {
        imgui_window->on_event(e);
    }
}

void ImguiLayer::on_update()
{
    auto window = _window.lock();

    ImGui::SetCurrentContext(_imgui_context);

    imgui_backend::new_frame(*window);
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

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
        for(auto& imgui_window : _imgui_window)
        {
            imgui_window->on_update();
        }
    }

    ImGui::Render();
    imgui_backend::render_draw_data(*window);

    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImguiLayer::scale_ui(float factor)
{
    ImGui::GetIO().FontGlobalScale = factor;
    ImGui::GetStyle().ScaleAllSizes(factor);
}

void ImguiLayer::_on_scale_imgui_window(events::ScaleImguiWindow& e)
{
    const auto& [scale] = e.data_tuple;
    scale_ui(scale);
}