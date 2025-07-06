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
    LayerStack& layer_stack = LayerStack::Instance();
    std::shared_ptr<WindowLayer> main_window_layer_sptr = layer_stack.GetLayerShared<WindowLayer>();
    std::shared_ptr<ImguiLayer> main_imgui_layer_sptr = layer_stack.GetLayerShared<ImguiLayer>();

    RuntimeConfig& runtime_config = RuntimeConfig::Instance();
    runtime_config.WindowSize = main_window_layer_sptr->GetSize();
    runtime_config.WindowPosition = main_window_layer_sptr->GetPosition();
    runtime_config.WindowMaxmized = main_window_layer_sptr->GetSizeState() == WindowSizeState::Maxmized;

    for(const std::unique_ptr<ImguiWindowInterface>& imgui_window : main_imgui_layer_sptr->GetImguiWindows())
    {
        if(imgui_window->Open())
        {
            runtime_config.ImguiOpenedWindowIdStrings.emplace(imgui_window->GetIdString());
        }
    }

    LayerStack::Instance().GetLayerShared<RuntimeLayer>()->Stop();
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