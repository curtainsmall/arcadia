#include "ui/ui_service.hpp"

#include "core/function.hpp"

#include "resource/fonts/icon.hpp"

#include "editor/editor/editor_app.hpp"
#include "ui/imgui_backend.hpp"

Arcadia::UiService::UiService(
    Engine& engine,
    const std::function<void(UiService&)>& imgui_window_installer,
    const std::function<void()>& imgui_style_setter,
    float init_scale
):
    _Engine(engine)
{
    _EventSubscription = _Engine.events.SubscribeAny(
        Foundation::Order::Imgui,
        [this](Foundation::Event& e) -> bool
        {
            OnEvent(e);
            return e.Handled();
        }
    );

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
    io.Fonts->AddFontFromFileTTF(FontFilepathString.data(), FontSize, &imgui_font_config, imgui_icon_ranges.data());

    ImguiBackend::Initialize(_Engine, *_Engine.window);

    imgui_style_setter();
    imgui_window_installer(*this);
}

Arcadia::UiService::~UiService()
{
    _EventSubscription.Reset();

    if(_pImguiContext)
    {
        ImguiBackend::Shutdown(_Engine, *_Engine.window);
        ImGui::DestroyContext(_pImguiContext);
    }
}

auto Arcadia::UiService::GetImguiWindows() const -> const std::vector<std::unique_ptr<ImguiWindowInterface>>&
{
    return _ImguiWindows;
}

void Arcadia::UiService::OnEvent(EventBase& e)
{
    // We do not dispatch events to ImGui when the editor is in play mode
    if(_Engine.editor != nullptr && _Engine.editor->IsInPlayMode())
    {
        return;
    }

    EventDispatcher(e)
        .Dispatch<Events::ScaleImguiWindow>(ACDA_BIND_MEMBER_FN(_OnScaleImguiWindow))
        .IsDispatched();

    ImguiBackend::OnEvent(_Engine, e);
    for(std::unique_ptr<ImguiWindowInterface>& imgui_window_uptr : _ImguiWindows)
    {
        imgui_window_uptr->OnEvent(e);
    }
}

void Arcadia::UiService::OnUpdate()
{
    WindowService* window_service = _Engine.window;

    ImGui::SetCurrentContext(_pImguiContext);

    ImguiBackend::NewFrame(_Engine, *window_service);
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGui::DockSpaceOverViewport();

    for(std::unique_ptr<ImguiWindowInterface>& imgui_window_uptr : _ImguiWindows)
    {
        imgui_window_uptr->OnUpdate();
    }

    ImGui::Render();
    ImguiBackend::RenderDrawData(_Engine, *window_service);

    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Arcadia::UiService::ScaleUi(float factor)
{
    ImGui::GetIO().FontGlobalScale = factor;
    ImGui::GetStyle().ScaleAllSizes(factor);
}

void Arcadia::UiService::_OnScaleImguiWindow(Events::ScaleImguiWindow& e)
{
    ScaleUi(e.NewScale);
}
