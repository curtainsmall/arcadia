#include "imgui_window_main_statusbar.hpp"

#include "core/runtime_config.hpp"

#include "editor/editor_layer.hpp"
#include "ui/imgui.hpp"

void Arcadia::ImguiWindowMainStatusbar::OnEvent(EventBase& e)
{
}

void Arcadia::ImguiWindowMainStatusbar::OnUpdate()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{ 0,0 });
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoFocusOnAppearing;
    if(ImGui::BeginViewportSideBar("statusbar", ImGui::GetMainViewport(), ImGuiDir_Down, ImGui::GetFrameHeight(), window_flags))
    {
        std::shared_ptr<EditorLayer> editor_layer_sptr = LayerStack::Instance().GetLayerShared<EditorLayer>();
        RuntimeConfig& runtime_config = RuntimeConfig::Instance();

        static const std::array<std::pair<std::string, float>, 3> scales{
            std::make_pair(std::string("70%"),0.7f),
            std::make_pair(std::string("100%"),1.0f),
            std::make_pair(std::string("150%"),1.5f)
        };
        static std::int32_t scale_idx = std::distance(
            scales.begin(),
            std::find_if(
                scales.begin(),
                scales.end(),
                [&](const std::pair<std::string, float>& pair) -> bool
                {
                    return pair.second == editor_layer_sptr->GetUiScale();
                }
            )
        );

        ImGuiComboFlags combo_flags =
            ImGuiComboFlags_NoPreview;
        //ImGui::SetNextWindowSizeConstraints({ -1,-1 }, { 20,50 });
        if(ImGui::BeginCombo(std::format("{}##ui_scale", scales[scale_idx].first).c_str(), nullptr, combo_flags))
        {
            std::int32_t new_idx = -1;
            for(auto& [string, factor] : scales)
            {
                new_idx++;
                if(ImGui::MenuItem(string.c_str()))
                {
                    editor_layer_sptr->SetUiScale(factor);
                    scale_idx = new_idx;
                }
            }

            if(runtime_config.UiScale != editor_layer_sptr->GetUiScale())
            {
                runtime_config.UiScale = editor_layer_sptr->GetUiScale();
                EventQueue::Instance().Signal<Events::ScaleImguiWindow>(editor_layer_sptr->GetUiScale());
            }

            ImGui::EndCombo();
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
}