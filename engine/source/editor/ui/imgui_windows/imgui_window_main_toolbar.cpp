#include "imgui_window_main_toolbar.hpp"

#include "core/command.hpp"
#include "core/event.hpp"
#include "core/function.hpp"
#include "resource/fonts/icon.hpp"
#include "resource/scene_layer.hpp"
#include "function/player/player_events.hpp"

#include "editor/editor_layer.hpp"
#include "ui/imgui.hpp"
#include "ui/ui_events.hpp"

Arcadia::ImguiWindowMainToolbar::ImguiWindowMainToolbar():
    ImguiWindowInterface(true, "Toolbar")
{
}

void Arcadia::ImguiWindowMainToolbar::OnEvent(EventBase& e)
{
}

void Arcadia::ImguiWindowMainToolbar::OnUpdate()
{
    auto [scene_layer_sptr, editor_layer_sptr] = LayerStack::Instance().GetMultipleLayersShared<SceneLayer, EditorLayer>();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{ 0,0 });
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoFocusOnAppearing;
    if(ImGui::BeginViewportSideBar("toolbar", ImGui::GetMainViewport(), ImGuiDir_Up, ImGui::GetFrameHeight(), window_flags))
    {
        CommandList& cmd_list = CommandList::Instance();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, glm::vec2{ 0,4 });
        if(ImGui::Button(ICON_FA_ARROW_CIRCLE_LEFT))
        {
            cmd_list.Undo();
        }
        ImGui::SetItemTooltip(" Undo ");
        ImGui::SameLine();
        auto combo_flags =
            ImGuiComboFlags_NoPreview;
        //ImGui::SetNextItemWidth(1.f);
        if(ImGui::BeginCombo("##undo_list", nullptr, combo_flags))
        {
            if(cmd_list.GetSize())
            {
                auto iter = cmd_list.begin();
                auto end = cmd_list.end();
                for(; iter != end; ++iter)
                {
                    if(cmd_list.IsCurrent(iter))
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
        ImGui::SetItemTooltip(" Undo List ");
        ImGui::PopStyleVar();
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_ARROW_CIRCLE_RIGHT))
        {
            cmd_list.Redo();
        }
        ImGui::SetItemTooltip(" Redo ");

        if(scene_layer_sptr->HasActiveScene())
        {
            ImGui::SameLine();
            if(editor_layer_sptr->IsInPlayMode())
            {
                ImGui::PushStyleColor(ImGuiCol_Text, glm::vec4{ 1.f,0.f,0.f,1.f });
                // implemented in Arcadia::EditorLayer::_OnInputKey()
                ImGui::Text("Press Shift + Esc to stop play mode");
                ImGui::PopStyleColor();
            }
            else
            {
                if(ImGui::Button("PLAY"))
                {
                    EventQueue::Instance()
                        .Signal<Events::SetPlayMode>(true)
                        .Signal<Events::PlayerControllerSetActive>(true);
                }
            }
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
}