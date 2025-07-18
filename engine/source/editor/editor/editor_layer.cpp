#include "editor_layer.hpp"

#include <memory>

#include "core/runtime_config.hpp"
#include "core/enum.hpp"
#include "core/event.hpp"
#include "core/function.hpp"
#include "core/layer.hpp"
#include "core/runtime_layer.hpp"

#include "ui/imgui_windows/imgui_window_main_manubar.hpp"
#include "ui/imgui_windows/imgui_window_main_statusbar.hpp"
#include "ui/imgui_windows/imgui_window_main_toolbar.hpp"
#include "ui/imgui_windows/imgui_window_outliner.hpp"
#include "ui/imgui_windows/imgui_window_property.hpp"
#include "ui/imgui_windows/imgui_window_state.hpp"
#include "ui/imgui_windows/imgui_window_viewport.hpp"
#include "ui/imgui_windows/imgui_window_console.hpp"

void Arcadia::EditorLayer::OnUpdate()
{
}

void Arcadia::EditorLayer::SetPlayMode(bool play_mode)
{
    _PlayMode = play_mode;
}

auto Arcadia::EditorLayer::GetPlayMode() const -> bool
{
    return _PlayMode;
}

void Arcadia::EditorLayer::SetUiScale(float ui_scale)
{
    _UiScale = ui_scale;
}

auto Arcadia::EditorLayer::GetUiScale() const -> float
{
    return _UiScale;
}

Arcadia::EditorLayer::EditorLayer()
{
    LayerStack& layer_stack = LayerStack::Instance();
    RuntimeConfig& runtime_config = RuntimeConfig::Instance();

    //Editor Layer
    SetUiScale(runtime_config.UiScale);

    // Project layer
    layer_stack.PushLayer<ProjectLayer>();

    // Editor ImGui layer
    layer_stack.PushLayer<ImguiLayer>(
        layer_stack.GetLayerShared<WindowLayer>(),
        [&](ImguiLayer& imgui_layer)
        {
            const std::set<std::string>& id_strings = runtime_config.ImguiOpenedWindowIdStrings;

            std::initializer_list<std::tuple<std::string, std::string>> imgui_window_ids{
                std::make_tuple(std::string("Outliner"),ImguiWindowOutliner::GetIdStringStatic()),
                std::make_tuple(std::string("Viewport"),ImguiWindowViewport::GetIdStringStatic()),
                std::make_tuple(std::string("Property"),ImguiWindowProperty::GetIdStringStatic()),
                std::make_tuple(std::string("State"),ImguiWindowState::GetIdStringStatic()),
                std::make_tuple(std::string("Console"), ImguiWindowConsole::GetIdStringStatic())
            };
            imgui_layer
                .EmplaceImguiWindow<ImguiWindowMainMenubar>(imgui_window_ids)
                .EmplaceImguiWindow<ImguiWindowMainToolbar>()
                .EmplaceImguiWindow<ImguiWindowMainStatusbar>()
                .EmplaceImguiWindow<ImguiWindowOutliner>(id_strings.contains(ImguiWindowOutliner::GetIdStringStatic()), "Outliner")
                .EmplaceImguiWindow<ImguiWindowViewport>(id_strings.contains(ImguiWindowViewport::GetIdStringStatic()), "Viewport")
                .EmplaceImguiWindow<ImguiWindowProperty>(id_strings.contains(ImguiWindowProperty::GetIdStringStatic()), "Property")
                .EmplaceImguiWindow<ImguiWindowState>(id_strings.contains(ImguiWindowState::GetIdStringStatic()), "State")
                .EmplaceImguiWindow<ImguiWindowConsole>(id_strings.contains(ImguiWindowConsole::GetIdStringStatic()), "Console");
        },
        ImguiStyle::SetToDark,
        GetUiScale()
    );

    EventQueue::Instance().Signal<Events::RuntimeStart>();
}

void Arcadia::EditorLayer::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::WindowShouldClose>(ACDA_BIND_MEMBER_FN(_OnWindowShouldClose))
        .Dispatch<Events::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .Dispatch<Events::TogglePlayMode>(ACDA_BIND_MEMBER_FN(_OnTogglePlayMode))
        .Dispatch<Events::InputKey>(ACDA_BIND_MEMBER_FN(_OnInputKey))
        .IsDispatched();
}

void Arcadia::EditorLayer::_Stop()
{
    auto [window_layer_sptr, imgui_layer_sptr] = LayerStack::Instance().GetMultipleLayersShared<WindowLayer, ImguiLayer>();

    RuntimeConfig& runtime_config = RuntimeConfig::Instance();
    runtime_config.WindowSize = window_layer_sptr->GetSize();
    runtime_config.WindowPosition = window_layer_sptr->GetPosition();
    runtime_config.WindowMaxmized = window_layer_sptr->GetSizeState() == WindowSizeState::Maxmized;

    for(const std::unique_ptr<ImguiWindowInterface>& imgui_window : imgui_layer_sptr->GetImguiWindows())
    {
        if(imgui_window->Open())
        {
            runtime_config.ImguiOpenedWindowIdStrings.emplace(imgui_window->GetIdString());
        }
    }

    EventQueue::Instance().Signal<Events::RuntimeStop>();
}

void Arcadia::EditorLayer::_OnWindowShouldClose(Events::WindowShouldClose& e)
{
    LayerStack& layer_stack = LayerStack::Instance();
    std::shared_ptr<WindowLayer> main_window_layer_sptr = layer_stack.GetLayerShared<WindowLayer>();
    std::shared_ptr<ProjectLayer> main_project_layer_sptr = layer_stack.GetLayerShared<ProjectLayer>();

    if(main_project_layer_sptr->HasProject())
    {
        _WaitingForProjectUnbuiltBeforeClosing = true;
    }
    else
    {
        _Stop();
    }
}

void Arcadia::EditorLayer::_OnProjectUnbuilt(Events::ProjectUnbuilt& e)
{
    if(_WaitingForProjectUnbuiltBeforeClosing)
    {
        _Stop();
    }
}

void Arcadia::EditorLayer::_OnWindowCloseCanceled(Events::WindowCloseCanceled& e)
{
    _WaitingForProjectUnbuiltBeforeClosing = false;
}

void Arcadia::EditorLayer::_OnTogglePlayMode(Events::TogglePlayMode& e)
{
    std::shared_ptr<EditorLayer> editor_layer_sptr = LayerStack::Instance().GetLayerShared<EditorLayer>();
    editor_layer_sptr->SetPlayMode(!editor_layer_sptr->GetPlayMode());
}

void Arcadia::EditorLayer::_OnInputKey(Events::InputKey& e)
{
    if(e.KeyCode == InputKey::Escape && !!(e.Modifier & InputModifier::Shift))
    {
        LayerStack::Instance().GetLayerShared<EditorLayer>()->SetPlayMode(false);
    }
}