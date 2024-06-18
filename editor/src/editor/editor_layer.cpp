#include "editor_layer.hpp"

#include<memory>

#include"core/app/app_config.hpp"
#include"core/app/app_context.hpp"
#include"core/entry_point.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_windows/imgui_window_main_manubar.hpp"
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

    // Window layer
    {
        editor_context.MainWindowLayer = layer_stack
            .PushLayer<WindowLayer>(
                app_config.WindowSize,
                app_config.WindowTitle,
                app_config.WindowMultisampleCount
            )
            .Top<WindowLayer>();
    }

    // Project layer
    {
        editor_context.MainProjectLayer = layer_stack
            .PushLayer<ProjectLayer>()
            .Top<ProjectLayer>();
    }

    // Editor ImGui layer
    {
        editor_context.MainImguiLayer = layer_stack
            .PushLayer<ImguiLayer>(
                editor_context.MainWindowLayer.lock(),
                ACDA_BIND_MEMBER_FN(_ImguiWindowInstaller),
                ImguiStyle::Dark
            )
            .Top<ImguiLayer>();

        //editor_context._wpMainImguiLayer.lock()->ShowDemoWindow = true;
    }
    app_context.Running = true;
}

void EditorAppLayer::OnUpdate()
{}

void EditorAppLayer::OnEvent(EventBase& event)
{
    EventDispatcher{ event }
        .Dispatch<Event::WindowShouldClose>(ACDA_BIND_MEMBER_FN(_OnWindowShouldClose))
        .Dispatch<Event::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .Dispatch<Event::PlayMode>(ACDA_BIND_MEMBER_FN(_OnPlayMode))
        .Dispatch<Event::InputKey>(ACDA_BIND_MEMBER_FN(_OnInputKey))
        .Result();
}

void EditorAppLayer::_ImguiWindowInstaller(ImguiLayer& imgui_layer)
{
    const auto& app_config = AppConfig::Instance();
    const auto& id_strs = app_config.ImguiOpenedWindowIdStrs;

    std::initializer_list<std::tuple<std::string, std::string>> imgui_window_ids{
        std::make_tuple("Outliner"s,ImguiWindowOutliner::GetIdStrStatic()),
        std::make_tuple("Viewport"s,ImguiWindowViewport::GetIdStrStatic()),
        std::make_tuple("Property"s,ImguiWindowProperty::GetIdStrStatic()),
        std::make_tuple("State"s,ImguiWindowState::GetIdStrStatic())
    };
    imgui_layer
        .EmplaceImguiWindow<ImguiWindowMainMenubar>(imgui_window_ids)
        .EmplaceImguiWindow<ImguiWindowMainToolbar>()
        .EmplaceImguiWindow<ImguiWindowOutliner>(id_strs.contains(ImguiWindowOutliner::GetIdStrStatic()), "Outliner")
        .EmplaceImguiWindow<ImguiWindowViewport>(id_strs.contains(ImguiWindowViewport::GetIdStrStatic()), "Viewport")
        .EmplaceImguiWindow<ImguiWindowProperty>(id_strs.contains(ImguiWindowProperty::GetIdStrStatic()), "Property")
        .EmplaceImguiWindow<ImguiWindowState>(id_strs.contains(ImguiWindowState::GetIdStrStatic()), "State");
}

void EditorAppLayer::_Stop()
{
    auto& editor_context = EditorContext::Instance();
    auto main_window_layer = editor_context.MainWindowLayer.lock();
    auto main_imgui_layer = editor_context.MainImguiLayer.lock();

    auto& app_config = AppConfig::Instance();
    app_config.WindowSize = main_window_layer->GetSize();
    app_config.WindowPos = main_window_layer->GetPos();
    app_config.WindowMaxmized = main_window_layer->GetSizeState() == WindowSizeState::Maxmized;

    for(const auto& imgui_window : main_imgui_layer->GetImguiWindow())
    {
        if(imgui_window->IsOpen())
        {
            app_config.ImguiOpenedWindowIdStrs.emplace(imgui_window->GetIdStr());
        }
    }

    auto& app_context = AppContext::Instance();
    app_context.Running = false;
}

void EditorAppLayer::_OnWindowShouldClose(Event::WindowShouldClose& e)
{
    auto& editor_context = EditorContext::Instance();
    auto main_window_layer = editor_context.MainWindowLayer.lock();
    auto main_project_layer = editor_context.MainProjectLayer.lock();

    const auto& [p_wnd] = e.data_tuple;
    if(p_wnd == main_window_layer.get() && main_project_layer->HasProject())
    {
        _WaitingForProjectUnbuiltBeforeClosing = true;
    }
    else
    {
        _Stop();
    }
}

void EditorAppLayer::_OnProjectUnbuilt(Event::ProjectUnbuilt& e)
{
    if(_WaitingForProjectUnbuiltBeforeClosing)
    {
        _Stop();
    }
}

void EditorAppLayer::_OnWindowCloseCanceled(Event::WindowCloseCanceled& e)
{
    _WaitingForProjectUnbuiltBeforeClosing = false;
}

void EditorAppLayer::_OnPlayMode(Event::PlayMode& e)
{
    const auto& [state] = e.data_tuple;
    EditorContext::Instance().InPlayMode = state;
}

void EditorAppLayer::_OnInputKey(Event::InputKey& e)
{
    const auto& [wnd, key, scancode, action, mods] = e.data_tuple;

    if(key == InputKey::Escape && mods & InputModifier::Shift)
    {
        EditorContext::Instance().InPlayMode = false;
    }

}

auto CreateApplication() -> std::unique_ptr<iAppLayer>
{
    return std::make_unique<EditorAppLayer>();
}
