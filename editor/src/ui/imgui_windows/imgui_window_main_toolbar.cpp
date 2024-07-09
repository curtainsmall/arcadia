#include "imgui_window_main_toolbar.hpp"

#include"core/event/event.hpp"
#include"core/memento/memento.hpp"
#include"resource/fonts/icon_header.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_header.hpp"
#include"ui/ui_events.hpp"

void ImguiWindowMainToolbar::on_event(EventBase& e)
{
    EventDispatcher{ e }
        .dispatch<events::SceneActivated>(ACDA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_on_scene_deactivated))
        .result();
}

void ImguiWindowMainToolbar::on_update()
{
    auto scene = _scene.lock();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{ 0,0 });
    auto window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoFocusOnAppearing;
    if(ImGui::BeginViewportSideBar("##toolbar", ImGui::GetMainViewport(), ImGuiDir_Up, ImGui::GetFrameHeight(), window_flags))
    {
        auto& memento_list = MementoList::instance();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, glm::vec2{ 0,4 });
        if(ImGui::Button(ICON_FA_ARROW_CIRCLE_LEFT))
        {
            memento_list.undo();
        }
        ImGui::SetItemTooltip(" Undo ");
        ImGui::SameLine();
        auto combo_flags =
            ImGuiComboFlags_NoPreview;
        //ImGui::SetNextItemWidth(1.f);
        if(ImGui::BeginCombo("##undo_list", nullptr, combo_flags))
        {
            if(memento_list.size())
            {
                auto iter = memento_list.begin();
                auto end = memento_list.end();
                for(; iter != end; ++iter)
                {
                    if(memento_list.is_current(iter))
                    {
                        ImGui::MenuItem(std::format("{} {}", ICON_FA_CHECK, iter->description()).c_str());
                    }
                    else
                    {
                        ImGui::MenuItem(std::format("  {}", iter->description()).c_str());
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
            memento_list.redo();
        }
        ImGui::SetItemTooltip(" Redo ");

        if(scene)
        {
            ImGui::SameLine();
            if(EditorContext::instance().in_play_mode)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, glm::vec4{ 1.f,0.f,0.f,1.f });
                ImGui::Text("Press Shift + Esc to stop play mode");
                ImGui::PopStyleColor();
            }
            else
            {
                if(ImGui::Button("PLAY"))
                {
                    EventQueue::instance().signal<events::PlayMode>(true);
                }
            }
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
}

void ImguiWindowMainToolbar::_on_scene_activated(events::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _scene = scene;
}


void ImguiWindowMainToolbar::_on_scene_deactivated(events::SceneDeactivated& e)
{
    _scene.reset();
}
