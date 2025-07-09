#include "core/main.hpp"
#include "core/layer.hpp"
#include "core/function.hpp"
#include "core/runtime_config.hpp"
#include "core/runtime_events.hpp"
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

ACDA_MAIN_FN(
    []()
    {
        Arcadia::LayerStack& layer_stack = Arcadia::LayerStack::Instance();
        Arcadia::RuntimeConfig& runtime_config = Arcadia::RuntimeConfig::Instance();

        //Editor Layer
        layer_stack.PushLayer<Arcadia::EditorLayer>();
        std::shared_ptr<Arcadia::EditorLayer> editor_layer_sptr = Arcadia::LayerStack::Instance().GetLayerShared<Arcadia::EditorLayer>();
        editor_layer_sptr->SetUiScale(runtime_config.UiScale);

        // Project layer
        layer_stack.PushLayer<Arcadia::ProjectLayer>();

        // Editor ImGui layer
        layer_stack.PushLayer<Arcadia::ImguiLayer>(
            layer_stack.GetLayerShared<Arcadia::WindowLayer>(),
            [&](Arcadia::ImguiLayer& imgui_layer)
            {
                const std::set<std::string>& id_strings = runtime_config.ImguiOpenedWindowIdStrings;

                std::initializer_list<std::tuple<std::string, std::string>> imgui_window_ids{
                    std::make_tuple(std::string("Outliner"),Arcadia::ImguiWindowOutliner::GetIdStringStatic()),
                    std::make_tuple(std::string("Viewport"),Arcadia::ImguiWindowViewport::GetIdStringStatic()),
                    std::make_tuple(std::string("Property"),Arcadia::ImguiWindowProperty::GetIdStringStatic()),
                    std::make_tuple(std::string("State"),Arcadia::ImguiWindowState::GetIdStringStatic())
                };
                imgui_layer
                    .EmplaceImguiWindow<Arcadia::ImguiWindowMainMenubar>(imgui_window_ids)
                    .EmplaceImguiWindow<Arcadia::ImguiWindowMainToolbar>()
                    .EmplaceImguiWindow<Arcadia::ImguiWindowMainStatusbar>()
                    .EmplaceImguiWindow<Arcadia::ImguiWindowOutliner>(id_strings.contains(Arcadia::ImguiWindowOutliner::GetIdStringStatic()), "Outliner")
                    .EmplaceImguiWindow<Arcadia::ImguiWindowViewport>(id_strings.contains(Arcadia::ImguiWindowViewport::GetIdStringStatic()), "Viewport")
                    .EmplaceImguiWindow<Arcadia::ImguiWindowProperty>(id_strings.contains(Arcadia::ImguiWindowProperty::GetIdStringStatic()), "Property")
                    .EmplaceImguiWindow<Arcadia::ImguiWindowState>(id_strings.contains(Arcadia::ImguiWindowState::GetIdStringStatic()), "State");
            },
            Arcadia::ImguiStyle::SetToDark
        );

        Arcadia::EventQueue::Instance().Signal<Arcadia::Events::RuntimeStart>();

    }
);