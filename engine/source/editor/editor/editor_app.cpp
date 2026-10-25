#include "editor/editor_app.hpp"

#include <memory>

#include "core/enum.hpp"
#include "core/event.hpp"
#include "core/function.hpp"
#include "app/runtime_config.hpp"
#include "function/physics/physics_events.hpp"
#include "function/player/player_events.hpp"

#include "ui/imgui_windows/imgui_window_console.hpp"
#include "ui/imgui_windows/imgui_window_main_manubar.hpp"
#include "ui/imgui_windows/imgui_window_main_statusbar.hpp"
#include "ui/imgui_windows/imgui_window_main_toolbar.hpp"
#include "ui/imgui_windows/imgui_window_outliner.hpp"
#include "ui/imgui_windows/imgui_window_property.hpp"
#include "ui/imgui_windows/imgui_window_state.hpp"
#include "ui/imgui_windows/imgui_window_viewport.hpp"

Arcadia::EditorApp::EditorApp(Engine& engine):
    _Engine(engine)
{
    _Engine.editor = this;

    _EventSubscription = _Engine.events.SubscribeAny(
        Foundation::Order::Editor,
        [this](Foundation::Event& e) -> bool
        {
            OnEvent(e);
            return e.Handled();
        }
    );

    // Editor
    SetUiScale(_Engine.config.UiScale);

    // Project
    _upProject = std::make_unique<ProjectService>(_Engine);
    _Engine.project = _upProject.get();

    // Editor ImGui
    _upUi = std::make_unique<UiService>(
        _Engine,
        [this](UiService& ui_service) -> void
        {
            const std::set<std::string>& id_strings = _Engine.config.OpenedWindowIds;

            std::initializer_list<std::tuple<std::string_view, std::string_view>> imgui_window_ids{
                std::make_tuple("Outliner",ImguiWindowOutliner::GetIdStringStatic()),
                std::make_tuple("Viewport",ImguiWindowViewport::GetIdStringStatic()),
                std::make_tuple("Property",ImguiWindowProperty::GetIdStringStatic()),
                std::make_tuple("State",ImguiWindowState::GetIdStringStatic()),
                std::make_tuple("Console", ImguiWindowConsole::GetIdStringStatic())
            };
            ui_service
                .EmplaceImguiWindow<ImguiWindowMainMenubar>(_Engine, imgui_window_ids)
                .EmplaceImguiWindow<ImguiWindowMainToolbar>(_Engine)
                .EmplaceImguiWindow<ImguiWindowMainStatusbar>(_Engine)
                .EmplaceImguiWindow<ImguiWindowOutliner>(_Engine, id_strings.contains(std::string(ImguiWindowOutliner::GetIdStringStatic())), "Outliner")
                .EmplaceImguiWindow<ImguiWindowViewport>(_Engine, id_strings.contains(std::string(ImguiWindowViewport::GetIdStringStatic())), "Viewport")
                .EmplaceImguiWindow<ImguiWindowProperty>(_Engine, id_strings.contains(std::string(ImguiWindowProperty::GetIdStringStatic())), "Property")
                .EmplaceImguiWindow<ImguiWindowState>(_Engine, id_strings.contains(std::string(ImguiWindowState::GetIdStringStatic())), "State")
                .EmplaceImguiWindow<ImguiWindowConsole>(_Engine, id_strings.contains(std::string(ImguiWindowConsole::GetIdStringStatic())), "Console");
        },
        ImguiStyle::SetToDark,
        GetUiScale()
    );
    _Engine.ui = _upUi.get();
}

Arcadia::EditorApp::~EditorApp()
{
    _EventSubscription.Reset();

    _Engine.ui = nullptr;
    _Engine.project = nullptr;
    _Engine.editor = nullptr;
}

void Arcadia::EditorApp::OnFrame(Engine& engine)
{
    engine.project->OnUpdate();
    engine.ui->OnUpdate();
    OnUpdate();
}

void Arcadia::EditorApp::OnStop(Engine& engine)
{
    _CollectConfig();
}

void Arcadia::EditorApp::OnUpdate()
{
}

void Arcadia::EditorApp::SetPlayMode(bool play_mode)
{
    _PlayMode = play_mode;
}

auto Arcadia::EditorApp::IsInPlayMode() const -> bool
{
    return _PlayMode;
}

void Arcadia::EditorApp::SetUiScale(float ui_scale)
{
    _UiScale = ui_scale;
}

auto Arcadia::EditorApp::GetUiScale() const -> float
{
    return _UiScale;
}

auto Arcadia::EditorApp::GetCsysSystem() -> csys::System&
{
    return _CsysSystem;
}

void Arcadia::EditorApp::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::WindowShouldClose>(ACDA_BIND_MEMBER_FN(_OnWindowShouldClose))
        .Dispatch<Events::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .Dispatch<Events::SetPlayMode>(ACDA_BIND_MEMBER_FN(_OnSetPlayMode))
        .Dispatch<Events::InputKey>(ACDA_BIND_MEMBER_FN(_OnInputKey))
        .IsDispatched();
}

void Arcadia::EditorApp::_Stop()
{
    _CollectConfig();

    _Engine.Stop();
}

void Arcadia::EditorApp::_CollectConfig()
{
    RuntimeConfig& runtime_config = _Engine.config;

    WindowService* window_service = _Engine.window;
    const glm::i32vec2 window_size = window_service->GetSize();
    const glm::i32vec2 window_position = window_service->GetPosition();
    runtime_config.WindowWidth = window_size.x;
    runtime_config.WindowHeight = window_size.y;
    runtime_config.WindowPositionX = window_position.x;
    runtime_config.WindowPositionY = window_position.y;
    runtime_config.WindowMaximized = window_service->GetSizeState() == GlfwWindowSizeState::Maxmized;

    runtime_config.OpenedWindowIds.clear();
    for(const std::unique_ptr<ImguiWindowInterface>& imgui_window_uptr : _Engine.ui->GetImguiWindows())
    {
        if(imgui_window_uptr->Open())
        {
            runtime_config.OpenedWindowIds.emplace(imgui_window_uptr->GetIdString());
        }
    }
}

void Arcadia::EditorApp::_OnWindowShouldClose(Events::WindowShouldClose& e)
{
    if(_Engine.project->HasProject())
    {
        _WaitingForProjectUnbuiltBeforeClosing = true;
    }
    else
    {
        _Stop();
    }
}

void Arcadia::EditorApp::_OnProjectUnbuilt(Events::ProjectUnbuilt& e)
{
    if(_WaitingForProjectUnbuiltBeforeClosing)
    {
        _Stop();
    }
}

void Arcadia::EditorApp::_OnWindowCloseCanceled(Events::WindowCloseCanceled& e)
{
    _WaitingForProjectUnbuiltBeforeClosing = false;
}

void Arcadia::EditorApp::_OnSetPlayMode(Events::SetPlayMode& e)
{
    SetPlayMode(e.InPlayMode);
}

void Arcadia::EditorApp::_OnInputKey(Events::InputKey& e)
{
    if(e.KeyCode == GlfwInputKey::Escape && !!(e.Modifier & GlfwInputModifier::Shift))
    {
        _Engine.events.Signal<Events::SetPlayMode>(false);
        _Engine.events.Signal<Events::PlayerControllerSetActive>(false);
        _Engine.events.Signal<Events::PhysicsSimulatorSetActive>(false);
    }
}
