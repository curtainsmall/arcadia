#include "editor_layer.hpp"

#include <memory>

#include "core/app/app_config.hpp"
#include "core/app/app_context.hpp"
#include "core/enum.hpp"
#include "core/event.hpp"
#include "core/function.hpp"
#include "core/layer.hpp"
#include "core/main.hpp"

#include "editor/editor_context.hpp"
#include "ui/imgui_windows/imgui_window_main_manubar.hpp"
#include "ui/imgui_windows/imgui_window_main_statusbar.hpp"
#include "ui/imgui_windows/imgui_window_main_toolbar.hpp"
#include "ui/imgui_windows/imgui_window_outliner.hpp"
#include "ui/imgui_windows/imgui_window_property.hpp"
#include "ui/imgui_windows/imgui_window_state.hpp"
#include "ui/imgui_windows/imgui_window_viewport.hpp"

Arcadia::EditorAppLayer::EditorAppLayer()
{
    LayerStack& layer_stack = LayerStack::Instance();
    AppConfig& app_config = AppConfig::Instance();
    AppContext& app_context = AppContext::Instance();
    EditorContext& editor_context = EditorContext::Instance();

    editor_context.UiScale = app_config.UiScale;

    // Window layer
    {
        editor_context.wpMainWindowLayer = layer_stack
            .PushLayer<WindowLayer>(
                app_config.WindowSize,
                app_config.WindowTitle,
                app_config.WindowMultisampleCount
            )
            .GetTopLayer<WindowLayer>();
    }

    // spProject layer
    {
        editor_context.wpMainProjectLayer = layer_stack
            .PushLayer<ProjectLayer>()
            .GetTopLayer<ProjectLayer>();
    }

    // Editor ImGui layer
    {
        editor_context.wpMainImguiLayer = layer_stack
            .PushLayer<ImguiLayer>(
                editor_context.wpMainWindowLayer.lock(),
                ACDA_BIND_MEMBER_FN(_InstallImguiWindow),
                ImguiStyle::SetToDark
            )
            .GetTopLayer<ImguiLayer>();
    }
    app_context.Running = true;
}

void Arcadia::EditorAppLayer::OnUpdate()
{}

void Arcadia::EditorAppLayer::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::WindowShouldClose>(ACDA_BIND_MEMBER_FN(_OnWindowShouldClose))
        .Dispatch<Events::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .Dispatch<Events::TogglePlayMode>(ACDA_BIND_MEMBER_FN(_OnTogglePlayMode))
        .Dispatch<Events::InputKey>(ACDA_BIND_MEMBER_FN(_OnInputKey))
        .IsDispatched();
}

void Arcadia::EditorAppLayer::_InstallImguiWindow(ImguiLayer& imgui_layer)
{
    const AppConfig& app_config = AppConfig::Instance();
    const std::set<std::string>& id_strings = app_config.ImguiOpenedWindowIdStrings;

    std::initializer_list<std::tuple<std::string, std::string>> imgui_window_ids{
        std::make_tuple(std::string("Outliner"),ImguiWindowOutliner::GetIdStringStatic()),
        std::make_tuple(std::string("Viewport"),ImguiWindowViewport::GetIdStringStatic()),
        std::make_tuple(std::string("Property"),ImguiWindowProperty::GetIdStringStatic()),
        std::make_tuple(std::string("State"),ImguiWindowState::GetIdStringStatic())
    };
    imgui_layer
        .EmplaceImguiWindow<ImguiWindowMainMenubar>(imgui_window_ids)
        .EmplaceImguiWindow<ImguiWindowMainToolbar>()
        .EmplaceImguiWindow<ImguiWindowMainStatusbar>()
        .EmplaceImguiWindow<ImguiWindowOutliner>(id_strings.contains(ImguiWindowOutliner::GetIdStringStatic()), "Outliner")
        .EmplaceImguiWindow<ImguiWindowViewport>(id_strings.contains(ImguiWindowViewport::GetIdStringStatic()), "Viewport")
        .EmplaceImguiWindow<ImguiWindowProperty>(id_strings.contains(ImguiWindowProperty::GetIdStringStatic()), "Property")
        .EmplaceImguiWindow<ImguiWindowState>(id_strings.contains(ImguiWindowState::GetIdStringStatic()), "State");
}

void Arcadia::EditorAppLayer::_Stop()
{
    EditorContext& editor_context = EditorContext::Instance();
    std::shared_ptr<WindowLayer> main_window_layer_sptr = editor_context.wpMainWindowLayer.lock();
    std::shared_ptr<ImguiLayer> main_imgui_layer_sptr = editor_context.wpMainImguiLayer.lock();

    AppConfig& app_config = AppConfig::Instance();
    app_config.WindowSize = main_window_layer_sptr->GetSize();
    app_config.WindowPosition = main_window_layer_sptr->GetPosition();
    app_config.WindowMaxmized = main_window_layer_sptr->GetSizeState() == WindowSizeState::Maxmized;

    for(const std::unique_ptr<ImguiWindowInterface>& imgui_window : main_imgui_layer_sptr->GetImguiWindow())
    {
        if(imgui_window->Open())
        {
            app_config.ImguiOpenedWindowIdStrings.emplace(imgui_window->GetIdString());
        }
    }

    AppContext& app_context = AppContext::Instance();
    app_context.Running = false;
}

void Arcadia::EditorAppLayer::_OnWindowShouldClose(Events::WindowShouldClose& e)
{
    EditorContext& editor_context = EditorContext::Instance();
    std::shared_ptr<WindowLayer> main_window_layer_sptr = editor_context.wpMainWindowLayer.lock();
    std::shared_ptr<ProjectLayer> main_project_layer_sptr = editor_context.wpMainProjectLayer.lock();

    if(e.pWindowLayer == main_window_layer_sptr.get() && main_project_layer_sptr->HasProject())
    {
        _WaitingForProjectUnbuiltBeforeClosing = true;
    }
    else
    {
        _Stop();
    }
}

void Arcadia::EditorAppLayer::_OnProjectUnbuilt(Events::ProjectUnbuilt& e)
{
    if(_WaitingForProjectUnbuiltBeforeClosing)
    {
        _Stop();
    }
}

void Arcadia::EditorAppLayer::_OnWindowCloseCanceled(Events::WindowCloseCanceled& e)
{
    _WaitingForProjectUnbuiltBeforeClosing = false;
}

void Arcadia::EditorAppLayer::_OnTogglePlayMode(Events::TogglePlayMode& e)
{
    EditorContext& instance = EditorContext::Instance();
    instance.InPlayMode = !instance.InPlayMode;
}

void Arcadia::EditorAppLayer::_OnInputKey(Events::InputKey& e)
{
    if(e.KeyCode == InputKey::Escape && !!(e.Modifier & InputModifier::Shift))
    {
        EditorContext::Instance().InPlayMode = false;
    }
}

auto Arcadia::CreateApplication() -> std::unique_ptr<AppLayerInterface>
{
    return std::make_unique<EditorAppLayer>();
}