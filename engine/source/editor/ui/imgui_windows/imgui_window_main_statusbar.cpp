#include "imgui_window_main_statusbar.hpp"

#include "app/runtime_config.hpp"

#include "editor/editor/editor_app.hpp"
#include "ui/imgui.hpp"

Arcadia::ImguiWindowMainStatusbar::ImguiWindowMainStatusbar(Engine& engine):
    ImguiWindowInterface(engine, true, "Statusbar")
{
}

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
        EditorApp* editor_app = _Engine.editor;
        RuntimeConfig& runtime_config = _Engine.config;

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
                    return pair.second == editor_app->GetUiScale();
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
                    editor_app->SetUiScale(factor);
                    scale_idx = new_idx;
                }
            }

            if(runtime_config.UiScale != editor_app->GetUiScale())
            {
                runtime_config.UiScale = editor_app->GetUiScale();
                _Engine.events.Signal<Events::ScaleImguiWindow>(editor_app->GetUiScale());
            }

            ImGui::EndCombo();
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
}
