#include "imgui_window_main_statusbar.hpp"

#include"core/app/app_config.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_header.hpp"

void Arcadia::ImguiWindowMainStatusbar::OnEvent(EventBase& e)
{}

void Arcadia::ImguiWindowMainStatusbar::OnUpdate()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{ 0,0 });
    auto window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoFocusOnAppearing;
    if(ImGui::BeginViewportSideBar("statusbar", ImGui::GetMainViewport(), ImGuiDir_Down, ImGui::GetFrameHeight(), window_flags))
    {
        auto combo_flags =
            ImGuiComboFlags_NoPreview;
        auto& editor_context = EditorContext::Instance();
        auto& app_config = AppConfig::Instance();

        static const std::array<std::pair<std::string, float>, 3> scales{
            std::make_pair("70%",0.7f),
            std::make_pair("100%",1.0f),
            std::make_pair("150%",1.5f)
        };
        static int32_t scale_idx = std::distance(
            scales.begin(),
            std::find_if(
                scales.begin(),
                scales.end(),
                [&](const std::pair<std::string, float>& pair)-> bool
        {
            return pair.second == editor_context.UiScale;
        }
            )
        );
        //ImGui::SetNextWindowSizeConstraints({ -1,-1 }, { 20,50 });
        if(ImGui::BeginCombo(std::format("{}##UiScale", scales[scale_idx].first).c_str(), nullptr, combo_flags))
        {
            int32_t new_idx = -1;
            for(auto& [str, factor] : scales)
            {
                new_idx++;
                if(ImGui::MenuItem(str.c_str()))
                {
                    editor_context.UiScale = factor;
                    scale_idx = new_idx;
                }
            }

            if(app_config.UiScale != editor_context.UiScale)
            {
                app_config.UiScale = editor_context.UiScale;
                EventQueue::Instance().Signal<Events::ScaleImguiWindow>(editor_context.UiScale);
            }

            ImGui::EndCombo();
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
}