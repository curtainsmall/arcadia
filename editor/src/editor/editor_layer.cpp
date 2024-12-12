#include "editor_layer.hpp"

#include<memory>

#include"core/app/app_config.hpp"
#include"core/app/app_context.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"
#include"core/main.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_windows/imgui_window_main_manubar.hpp"
#include"ui/imgui_windows/imgui_window_main_statusbar.hpp"
#include"ui/imgui_windows/imgui_window_main_toolbar.hpp"
#include"ui/imgui_windows/imgui_window_outliner.hpp"
#include"ui/imgui_windows/imgui_window_property.hpp"
#include"ui/imgui_windows/imgui_window_state.hpp"
#include"ui/imgui_windows/imgui_window_viewport.hpp"

EditorAppLayer::EditorAppLayer()
{
    auto& layer_stack = LayerStack::Instance();
    const auto& app_config = AppConfig::Instance();
    auto& app_context = AppContext::Instance();
    auto& editor_context = EditorContext::Instance();

    editor_context.UiScale = app_config.UiScale;

    // Window layer
    {
        editor_context.MainWindowLayer = layer_stack
            .PushLayer<WindowLayer>(
                app_config.WindowSize,
                app_config.WindowTitle,
                app_config.WindowMultisampleCount
            )
            .GetTopLayer<WindowLayer>();
    }

    // Project layer
    {
        editor_context.MainProjectLayer = layer_stack
            .PushLayer<ProjectLayer>()
            .GetTopLayer<ProjectLayer>();
    }

    // Editor ImGui layer
    {
        editor_context.MainImguiLayer = layer_stack
            .PushLayer<ImguiLayer>(
                editor_context.MainWindowLayer.lock(),
                ACDA_BIND_MEMBER_FN(_InstallImguiWindow),
                ImguiStyle::SetToDark
            )
            .GetTopLayer<ImguiLayer>();

        //editor_context._wpmain_imgui_layer.lock()->ShowDemoWindow = true;
    }
    app_context.Running = true;
}

void EditorAppLayer::OnUpdate()
{}

void EditorAppLayer::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::WindowShouldClose>(ACDA_BIND_MEMBER_FN(_OnWindowShouldClose))
        .Dispatch<Events::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .Dispatch<Events::TogglePlayMode>(ACDA_BIND_MEMBER_FN(_OnTogglePlayMode))
        .Dispatch<Events::InputKey>(ACDA_BIND_MEMBER_FN(_OnInputKey))
        .IsDispatched();
}

void EditorAppLayer::_InstallImguiWindow(ImguiLayer& imgui_layer)
{
    const auto& app_config = AppConfig::Instance();
    const auto& id_strs = app_config.ImguiOpenedWindowIdStrings;

    std::initializer_list<std::tuple<std::string, std::string>> imgui_window_ids{
        std::make_tuple("Outliner"s,ImguiWindowOutliner::GetIdStringStatic()),
        std::make_tuple("Viewport"s,ImguiWindowViewport::GetIdStringStatic()),
        std::make_tuple("Property"s,ImguiWindowProperty::GetIdStringStatic()),
        std::make_tuple("State"s,ImguiWindowState::GetIdStringStatic())
    };
    imgui_layer
        .EmplaceImguiWindow<ImguiWindowMainMenubar>(imgui_window_ids)
        .EmplaceImguiWindow<ImguiWindowMainToolbar>()
        .EmplaceImguiWindow<ImguiWindowMainStatusbar>()
        .EmplaceImguiWindow<ImguiWindowOutliner>(id_strs.contains(ImguiWindowOutliner::GetIdStringStatic()), "Outliner")
        .EmplaceImguiWindow<ImguiWindowViewport>(id_strs.contains(ImguiWindowViewport::GetIdStringStatic()), "Viewport")
        .EmplaceImguiWindow<ImguiWindowProperty>(id_strs.contains(ImguiWindowProperty::GetIdStringStatic()), "Property")
        .EmplaceImguiWindow<ImguiWindowState>(id_strs.contains(ImguiWindowState::GetIdStringStatic()), "State");
}

void EditorAppLayer::_Stop()
{
    auto& editor_context = EditorContext::Instance();
    auto main_window_layer = editor_context.MainWindowLayer.lock();
    auto main_imgui_layer = editor_context.MainImguiLayer.lock();

    auto& app_config = AppConfig::Instance();
    app_config.WindowSize = main_window_layer->GetSize();
    app_config.WindowPosition = main_window_layer->GetPosition();
    app_config.WindowMaxmized = main_window_layer->GetSizeState() == WindowSizeState::Maxmized;

    for(const auto& imgui_window : main_imgui_layer->GetImguiWindow())
    {
        if(imgui_window->Open())
        {
            app_config.ImguiOpenedWindowIdStrings.emplace(imgui_window->GetIdString());
        }
    }

    auto& app_context = AppContext::Instance();
    app_context.Running = false;
}

void EditorAppLayer::_OnWindowShouldClose(Events::WindowShouldClose& e)
{
    auto& editor_context = EditorContext::Instance();
    auto main_window_layer = editor_context.MainWindowLayer.lock();
    auto main_project_layer = editor_context.MainProjectLayer.lock();

    const auto& [p_wnd] = e.DataTuple;
    if(p_wnd == main_window_layer.get() && main_project_layer->HasProject())
    {
        _WaitingForProjectUnbuiltBeforeClosing = true;
    }
    else
    {
        _Stop();
    }
}

void EditorAppLayer::_OnProjectUnbuilt(Events::ProjectUnbuilt& e)
{
    if(_WaitingForProjectUnbuiltBeforeClosing)
    {
        _Stop();
    }
}

void EditorAppLayer::_OnWindowCloseCanceled(Events::WindowCloseCanceled& e)
{
    _WaitingForProjectUnbuiltBeforeClosing = false;
}

void EditorAppLayer::_OnTogglePlayMode(Events::TogglePlayMode& e)
{
    const auto& [state] = e.DataTuple;
    EditorContext::Instance().InPlayMode = state;
}

void EditorAppLayer::_OnInputKey(Events::InputKey& e)
{
    const auto& [wnd, key, scancode, action, mods] = e.DataTuple;

    if(key == InputKey::Escape && ToBool(mods & InputModifier::Shift))
    {
        EditorContext::Instance().InPlayMode = false;
    }
}

auto CreateApplication() -> std::unique_ptr<iAppLayer>
{
    return std::make_unique<EditorAppLayer>();
}