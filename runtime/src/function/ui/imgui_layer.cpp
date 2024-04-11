#include"pch.hpp"
#include "imgui_layer.hpp"

#include"function/input/input_events.hpp"
#include"function/ui/imgui_backend.hpp"
#include"resource/fonts/icon_header.hpp"

Arcadia::ImguiLayer::ImguiLayer(
    const std::shared_ptr<const Arcadia::WindowLayer>& window_layer_sptr,
    const std::function<void(Arcadia::ImguiLayer&)>& imgui_window_installer,
    const std::function<void()>& imgui_style_setter
):
    Arcadia::iLayer("imgui"),
    _wpWindow(window_layer_sptr)
{
    _pImguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(_pImguiContext);

    auto& io = _pImguiContext->IO;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags =
        ImGuiConfigFlags_DockingEnable
        | ImGuiConfigFlags_NoMouseCursorChange
        | ImGuiConfigFlags_ViewportsEnable;
    io.Fonts->AddFontDefault();
    ImFontConfig imgui_font_config{};
    imgui_font_config.MergeMode = true;
    static const ImWchar imgui_icon_ranges[] ={ ICON_MIN_FA, ICON_MAX_FA,0 };
    io.Fonts->AddFontFromFileTTF(Arcadia::FontFilepathStr.c_str(), Arcadia::FontSize, &imgui_font_config, imgui_icon_ranges);
    Arcadia::imgui_backend::Initialize(*_wpWindow.lock());

    imgui_style_setter();

    imgui_window_installer(*this);
}

Arcadia::ImguiLayer::~ImguiLayer()
{
    if(_pImguiContext)
    {
        Arcadia::imgui_backend::Shutdown(*_wpWindow.lock());
        ImGui::DestroyContext(_pImguiContext);
    }
}

void Arcadia::ImguiLayer::OnEvent(Arcadia::EventBase& event)
{
    Arcadia::imgui_backend::ImguiOnEvent(event);
    for(auto& imgui_window_uptr : _upImguiWindow)
    {
        imgui_window_uptr->OnEvent(event);
    }
}

void Arcadia::ImguiLayer::OnUpdate()
{
    auto window_sptr = _wpWindow.lock();

    ImGui::SetCurrentContext(_pImguiContext);

    Arcadia::imgui_backend::NewFrame(*window_sptr);
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport();

    if(ShowDebugInfo)
    {
        ImGui::ShowStackToolWindow();
        ImGui::ShowMetricsWindow();
    }

    if(ShowDemoWindow)
    {
        ImGui::ShowDemoWindow();
    }
    else
    {

        for(auto& imgui_window_uptr : _upImguiWindow)
        {
            imgui_window_uptr->OnUpdate();
        }
    }

    ImGui::Render();
    Arcadia::imgui_backend::RenderDrawData(*window_sptr);

    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
