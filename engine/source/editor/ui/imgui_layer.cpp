#include "imgui_layer.hpp"

#include "core/function.hpp"
#include "function/input/input_events.hpp"
#include "resource/fonts/icon.hpp"

#include "editor/editor_layer.hpp"
#include "ui/imgui_backend.hpp"

Arcadia::ImguiLayer::ImguiLayer(
    const std::shared_ptr<WindowLayer>& window_layer_sptr,
    const std::function<void(ImguiLayer&)>& imgui_window_installer,
    const std::function<void()>& imgui_style_setter,
    float init_scale
):
    LayerInterface("imgui"),
    _wpWindow(window_layer_sptr)
{
    _pImguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(_pImguiContext);

    ScaleUi(init_scale);

    ImGuiIO& io = _pImguiContext->IO;
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

    ImguiBackend::Initialize(_wpWindow.lock());

    imgui_style_setter();
    imgui_window_installer(*this);
}

Arcadia::ImguiLayer::~ImguiLayer()
{
    if(_pImguiContext)
    {
        ImguiBackend::Shutdown(_wpWindow.lock());
        ImGui::DestroyContext(_pImguiContext);
    }
}

auto Arcadia::ImguiLayer::GetWindow() const -> std::shared_ptr<WindowLayer>
{
    return _wpWindow.lock();
}

auto Arcadia::ImguiLayer::GetImguiWindows() const -> const std::vector<std::unique_ptr<ImguiWindowInterface>>&
{
    return _ImguiWindows;
}

void Arcadia::ImguiLayer::OnEvent(EventBase& e)
{
    std::shared_ptr<EditorLayer> editor_layer_sptr = LayerStack::Instance().GetLayerShared<EditorLayer>();

    // We do not dispatch events to ImGui when the editor is in play mode
    if(editor_layer_sptr->GetPlayMode())
    {
        return;
    }

    EventDispatcher(e)
        .Dispatch<Events::ScaleImguiWindow>(ACDA_BIND_MEMBER_FN(_OnScaleImguiWindow))
        .IsDispatched();

    ImguiBackend::OnEvent(e);
    for(std::unique_ptr<ImguiWindowInterface>& imgui_window_uptr : _ImguiWindows)
    {
        imgui_window_uptr->OnEvent(e);
    }
}

void Arcadia::ImguiLayer::OnUpdate()
{
    std::shared_ptr<WindowLayer> window_sptr = _wpWindow.lock();

    ImGui::SetCurrentContext(_pImguiContext);

    ImguiBackend::NewFrame(window_sptr);
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
        for(std::unique_ptr<ImguiWindowInterface>& imgui_window_uptr : _ImguiWindows)
        {
            imgui_window_uptr->OnUpdate();
        }
    }

    ImGui::Render();
    ImguiBackend::RenderDrawData(window_sptr);

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
    ScaleUi(e.NewScale);
}