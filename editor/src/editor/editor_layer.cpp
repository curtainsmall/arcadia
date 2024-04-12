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

Arcadia::EditorAppLayer::EditorAppLayer()
{
    auto& layer_stack = Arcadia::LayerStack::Instance();
    const auto& app_config = Arcadia::AppConfig::Instance();
    auto& app_context = Arcadia::AppContext::Instance();
    auto& editor_context = Arcadia::EditorContext::Instance();

    // Window layer
    {
        editor_context._wpMainWindowLayer = layer_stack
            .PushLayer<Arcadia::WindowLayer>(
                app_config.WindowSize,
                app_config.WindowTitle,
                app_config.WindowMultisampleCount
            )
            .Top<Arcadia::WindowLayer>();
    }

    // Project layer
    {
        editor_context._wpMainProjectLayer = layer_stack
            .PushLayer<Arcadia::ProjectLayer>()
            .Top<Arcadia::ProjectLayer>();
    }

    // Editor ImGui layer
    {
        editor_context._wpMainImguiLayer = layer_stack
            .PushLayer<Arcadia::ImguiLayer>(
                editor_context._wpMainWindowLayer.lock(),
                ARCADIA_BIND_MEMBER_FN(_ImguiWindowInstaller)
            )
            .Top<Arcadia::ImguiLayer>();

        //editor_context.main_imgui_layer_wptr.lock()->show_demo_window = true;
    }
    app_context.Running = true;
}

void Arcadia::EditorAppLayer::OnUpdate()
{}

void Arcadia::EditorAppLayer::OnEvent(Arcadia::EventBase& event)
{
    Arcadia::EventDispatcher{ event }
        .Dispatch<Arcadia::Event::WindowShouldClose>(ARCADIA_BIND_MEMBER_FN(_OnWindowShouldClose))
        .Dispatch<Arcadia::Event::ProjectUnbuilt>(ARCADIA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .Result();
}

void Arcadia::EditorAppLayer::_ImguiWindowInstaller(Arcadia::ImguiLayer& imgui_layer)
{
    const auto& app_config = Arcadia::AppConfig::Instance();
    const auto& id_strs = app_config.ImguiOpenedWindowIdStrs;

    std::initializer_list<std::tuple<std::string, std::string>> imgui_window_ids{
        std::make_tuple("Outliner"s,Arcadia::ImguiWindowOutliner::GetIdStrStatic()),
        std::make_tuple("Viewport"s,Arcadia::ImguiWindowViewport::GetIdStrStatic()),
        std::make_tuple("Property"s,Arcadia::ImguiWindowProperty::GetIdStrStatic()),
        std::make_tuple("State"s,Arcadia::ImguiWindowState::GetIdStrStatic())
    };
    imgui_layer
        .EmplaceImguiWindow<Arcadia::ImguiWindowMainMenubar>(imgui_window_ids)
        .EmplaceImguiWindow<Arcadia::ImguiWindowMainToolbar>()
        .EmplaceImguiWindow<Arcadia::ImguiWindowOutliner>(id_strs.contains(Arcadia::ImguiWindowOutliner::GetIdStrStatic()), "Outliner")
        .EmplaceImguiWindow<Arcadia::ImguiWindowViewport>(id_strs.contains(Arcadia::ImguiWindowViewport::GetIdStrStatic()), "Viewport")
        .EmplaceImguiWindow<Arcadia::ImguiWindowProperty>(id_strs.contains(Arcadia::ImguiWindowProperty::GetIdStrStatic()), "Property")
        .EmplaceImguiWindow<Arcadia::ImguiWindowState>(id_strs.contains(Arcadia::ImguiWindowState::GetIdStrStatic()), "State");
}

void Arcadia::EditorAppLayer::_Stop()
{
    auto& editor_context = Arcadia::EditorContext::Instance();
    auto sp_main_window_layer = editor_context._wpMainWindowLayer.lock();
    auto sp_main_imgui_layer = editor_context._wpMainImguiLayer.lock();

    auto& app_config = Arcadia::AppConfig::Instance();
    app_config.WindowSize = sp_main_window_layer->GetSize();
    app_config.WindowPos = sp_main_window_layer->GetPos();
    app_config.WindowMaxmized = sp_main_window_layer->GetSizeState() == Arcadia::WindowSizeState::Maxmized;

    for(const auto& up_imgui_window : sp_main_imgui_layer->get_imgui_window_uptrs())
    {
        if(up_imgui_window->IsOpen())
        {
            app_config.ImguiOpenedWindowIdStrs.emplace(up_imgui_window->GetIdStr());
        }
    }

    auto& app_context = Arcadia::AppContext::Instance();
    app_context.Running = false;
}

void Arcadia::EditorAppLayer::_OnWindowShouldClose(Arcadia::Event::WindowShouldClose& e)
{
    auto& editor_context = Arcadia::EditorContext::Instance();
    auto sp_main_window_layer = editor_context._wpMainWindowLayer.lock();
    auto sp_main_project_layer = editor_context._wpMainProjectLayer.lock();

    const auto& [p_wnd] = e.data_tuple;
    if(p_wnd == sp_main_window_layer.get() && sp_main_project_layer->HasProject())
    {
        _WaitingForProjectUnbuiltBeforeClosing = true;
    }
    else
    {
        _Stop();
    }
}

void Arcadia::EditorAppLayer::_OnProjectUnbuilt(Arcadia::Event::ProjectUnbuilt& e)
{
    if(_WaitingForProjectUnbuiltBeforeClosing)
    {
        _Stop();
    }
}

void Arcadia::EditorAppLayer::_OnWindowCloseCanceled(Arcadia::Event::WindowCloseCanceled& e)
{
    _WaitingForProjectUnbuiltBeforeClosing = false;
}

auto Arcadia::CreateApplicationUptr() -> std::unique_ptr<Arcadia::iAppLayer>
{
    return std::make_unique<Arcadia::EditorAppLayer>();
}
