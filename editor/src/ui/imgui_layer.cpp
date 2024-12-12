#include"pch.hpp"

#include "imgui_layer.hpp"

#include"function/input/input_events.hpp"
#include"resource/fonts/icon_header.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_backend.hpp"

Arcadia::ImguiLayer::ImguiLayer(
    const std::shared_ptr<const WindowLayer>& window_layer,
    const std::function<void(ImguiLayer&)>& imgui_window_installer,
    const std::function<void()>& imgui_style_setter
) :
    iLayer("imgui"),
    _Window(window_layer)
{
    _ImguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(_ImguiContext);

    ScaleUi(EditorContext::Instance().UiScale);

    auto& io = _ImguiContext->IO;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags =
        ImGuiConfigFlags_DockingEnable
        | ImGuiConfigFlags_NoMouseCursorChange
        | ImGuiConfigFlags_ViewportsEnable;
    io.Fonts->AddFontDefault();

    ImFontConfig imgui_font_config{};
    imgui_font_config.MergeMode = true;
    static const std::array<ImWchar, 3> imgui_icon_ranges{ ICON_MIN_FA, ICON_MAX_FA,0 };
    io.Fonts->AddFontFromFileTTF(FontFilepathString.c_str(), FontSize, &imgui_font_config, imgui_icon_ranges.data());

    ImguiBackend::Initialize(*_Window.lock());

    imgui_style_setter();
    imgui_window_installer(*this);
}

Arcadia::ImguiLayer::~ImguiLayer()
{
    if(_ImguiContext)
    {
        ImguiBackend::Shutdown(*_Window.lock());
        ImGui::DestroyContext(_ImguiContext);
    }
}

void Arcadia::ImguiLayer::OnEvent(EventBase& e)
{
    // We do not dispatch events to ImGui when the editor is in play mode
    if(EditorContext::Instance().InPlayMode)
    {
        return;
    }

    EventDispatcher{ e }
        .Dispatch<Events::ScaleImguiWindow>(ACDA_BIND_MEMBER_FN(_OnScaleImguiWindow))
        .IsDispatched();

    ImguiBackend::OnEvent(e);
    for(auto& imgui_window : _ImguiWindow)
    {
        imgui_window->OnEvent(e);
    }
}

void Arcadia::ImguiLayer::OnUpdate()
{
    auto window = _Window.lock();

    ImGui::SetCurrentContext(_ImguiContext);

    ImguiBackend::NewFrame(*window);
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGui::DockSpaceOverViewport();

    if(ShouldShowDebugInfo)
    {
        ImGui::ShowStackToolWindow();
        ImGui::ShowMetricsWindow();
    }

    if(ShouldShowDemoWindow)
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
    ImguiBackend::RenderDrawData(*window);

    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Arcadia::ImguiLayer::ScaleUi(float factor)
{
    ImGui::GetIO().FontGlobalScale = factor;
    ImGui::GetStyle().ScaleAllSizes(factor);
}

void Arcadia::ImguiLayer::_OnScaleImguiWindow(Events::ScaleImguiWindow& e)
{
    const auto& [scale] = e.DataTuple;
    ScaleUi(scale);
}