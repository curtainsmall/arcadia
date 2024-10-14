#include "imgui_window_main_statusbar.hpp"

#include"core/app/app_config.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_header.hpp"

void ImguiWindowMainStatusbar::on_event(EventBase& e)
{}

void ImguiWindowMainStatusbar::on_update()
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
        auto& editor_context = EditorContext::instance();
        auto& app_config = AppConfig::instance();

        static const std::array<std::pair<std::string, float>, 3> scales{
            std::make_pair("70%",0.7f),
            std::make_pair("100%",1.0f),
            std::make_pair("150%",1.5f)
        };
        static int scale_idx = std::distance(
            scales.begin(),
            std::find_if(
                scales.begin(),
                scales.end(),
                [&](const std::pair<std::string, float>& pair)-> bool
        {
            return pair.second == editor_context.ui_scale;
        }
            )
        );
        //ImGui::SetNextWindowSizeConstraints({ -1,-1 }, { 20,50 });
        if(ImGui::BeginCombo(std::format("{}##ui_scale", scales[scale_idx].first).c_str(), nullptr, combo_flags))
        {
            int new_idx = -1;
            for(auto& [str, factor] : scales)
            {
                new_idx++;
                if(ImGui::MenuItem(str.c_str()))
                {
                    editor_context.ui_scale = factor;
                    scale_idx = new_idx;
                }
            }

            if(app_config.ui_scale != editor_context.ui_scale)
            {
                app_config.ui_scale = editor_context.ui_scale;
                EventQueue::instance().signal<events::ScaleImguiWindow>(editor_context.ui_scale);
            }

            ImGui::EndCombo();
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
}