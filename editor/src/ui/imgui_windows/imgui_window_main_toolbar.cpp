#include "imgui_window_main_toolbar.hpp"

#include"core/memento/memento.hpp"
#include"function/ui/imgui_header.hpp"
#include"resource/fonts/icon_header.hpp"

void Arcadia::ImguiWindowMainToolbar::OnEvent(Arcadia::EventBase& event)
{}

void Arcadia::ImguiWindowMainToolbar::OnUpdate()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{ 0,0 });
    auto window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoFocusOnAppearing;
    if(ImGui::BeginViewportSideBar("##toolbar", ImGui::GetMainViewport(), ImGuiDir_Up, ImGui::GetFrameHeight(), window_flags))
    {
        auto& memento_list = Arcadia::MementoList::Instance();
        if(ImGui::Button(ICON_FA_ARROW_CIRCLE_LEFT))
        {
            memento_list.Undo();
        }
        ImGui::SameLine();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, glm::vec2{ 0,4 });
        auto combo_flags =
            ImGuiComboFlags_NoPreview;
        if(ImGui::BeginCombo("##undo_list", nullptr, combo_flags))
        {
            if(memento_list.Size())
            {
                for(auto iter = memento_list.begin(); iter != memento_list.end(); ++iter)
                {

                    if(memento_list.IsCurrent(iter))
                    {
                        ImGui::MenuItem(std::format("{} {}", ICON_FA_CHECK, iter->GetDescription()).c_str());
                    }
                    else
                    {
                        ImGui::MenuItem(std::format("  {}", iter->GetDescription()).c_str());
                    }
                }
            }
            else
            {
                ImGui::MenuItem("(Empty undo List)");
            }

            ImGui::EndCombo();
        }
        ImGui::PopStyleVar();
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_ARROW_CIRCLE_RIGHT))
        {
            memento_list.Redo();
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
}
