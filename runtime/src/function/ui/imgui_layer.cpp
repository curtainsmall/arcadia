#include"pch.hpp"
#include "imgui_layer.hpp"

#include"function/input/input_events.hpp"
#include"function/ui/imgui_backend.hpp"
#include"resource/fonts/icon_header.hpp"

Arcadia::ImguiLayer::ImguiLayer(
    const std::shared_ptr<const Arcadia::WindowLayer>& window_layer,
    const std::function<void(Arcadia::ImguiLayer&)>& imgui_window_installer,
    const std::function<void()>& imgui_style_setter
):
    Arcadia::iLayer("imgui"),
    _Window(window_layer)
{
    _ImguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(_ImguiContext);

    auto& io = _ImguiContext->IO;
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
    Arcadia::imgui_backend::Initialize(*_Window.lock());

    imgui_style_setter();

    imgui_window_installer(*this);
}

Arcadia::ImguiLayer::~ImguiLayer()
{
    if(_ImguiContext)
    {
        Arcadia::imgui_backend::Shutdown(*_Window.lock());
        ImGui::DestroyContext(_ImguiContext);
    }
}

void Arcadia::ImguiLayer::OnEvent(Arcadia::EventBase& event)
{
    Arcadia::imgui_backend::ImguiOnEvent(event);
    for(auto& imgui_window : _ImguiWindow)
    {
        imgui_window->OnEvent(event);
    }
}

void Arcadia::ImguiLayer::OnUpdate()
{
    auto window = _Window.lock();

    ImGui::SetCurrentContext(_ImguiContext);

    Arcadia::imgui_backend::NewFrame(*window);
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

        for(auto& imgui_window : _ImguiWindow)
        {
            imgui_window->OnUpdate();
        }
    }

    ImGui::Render();
    Arcadia::imgui_backend::RenderDrawData(*window);

    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
