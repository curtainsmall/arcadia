#include "core/main.hpp"
#include "core/layer.hpp"
#include "core/function.hpp"
#include "core/runtime_config.hpp"
#include "core/event.hpp"

#include "editor/editor_layer.hpp"
#include "project/project_layer.hpp"
#include "ui/imgui_layer.hpp"
#include "ui/imgui_windows/imgui_window_main_manubar.hpp"
#include "ui/imgui_windows/imgui_window_main_statusbar.hpp"
#include "ui/imgui_windows/imgui_window_main_toolbar.hpp"
#include "ui/imgui_windows/imgui_window_outliner.hpp"
#include "ui/imgui_windows/imgui_window_property.hpp"
#include "ui/imgui_windows/imgui_window_state.hpp"
#include "ui/imgui_windows/imgui_window_viewport.hpp"

void Arcadia::ApplicationStartup()
{
    LayerStack& layer_stack = LayerStack::Instance();
    RuntimeConfig& runtime_config = RuntimeConfig::Instance();

    //Editor Layer
    layer_stack.PushLayer<EditorLayer>();
    std::shared_ptr<EditorLayer> editor_layer_sptr = LayerStack::Instance().GetLayerShared<EditorLayer>();
    editor_layer_sptr->SetUiScale(runtime_config.UiScale);

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
        },
        ImguiStyle::SetToDark
    );

    EventQueue::Instance().Signal<Events::RuntimeStart>();
}