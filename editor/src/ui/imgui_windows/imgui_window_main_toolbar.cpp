#include "imgui_window_main_toolbar.hpp"

#include"core/event/event.hpp"
#include"core/memento/memento.hpp"
#include"resource/fonts/icon_header.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_header.hpp"
#include"ui/ui_events.hpp"

void ImguiWindowMainToolbar::OnEvent(EventBase& event)
{
    EventDispatcher{ event }
        .Dispatch<Event::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Event::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .Result();
}

void ImguiWindowMainToolbar::OnUpdate()
{
    auto scene = _Scene.lock();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{ 0,0 });
    auto window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoFocusOnAppearing;
    if(ImGui::BeginViewportSideBar("##toolbar", ImGui::GetMainViewport(), ImGuiDir_Up, ImGui::GetFrameHeight(), window_flags))
    {
        auto& memento_list = MementoList::Instance();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, glm::vec2{ 0,4 });
        if(ImGui::Button(ICON_FA_ARROW_CIRCLE_LEFT))
        {
            memento_list.Undo();
        }
        ImGui::SetItemTooltip(" Undo ");
        ImGui::SameLine();
        auto combo_flags =
            ImGuiComboFlags_NoPreview;
        //ImGui::SetNextItemWidth(1.f);
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
        ImGui::SetItemTooltip(" Undo List ");
        ImGui::PopStyleVar();
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_ARROW_CIRCLE_RIGHT))
        {
            memento_list.Redo();
        }
        ImGui::SetItemTooltip(" Redo ");

        if(scene)
        {
            ImGui::SameLine();
            if(EditorContext::Instance().InPlayMode)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, glm::vec4{ 1.f,0.f,0.f,1.f });
                ImGui::Text("Press Shift + Esc to stop play mode");
                ImGui::PopStyleColor();
            }
            else
            {
                if(ImGui::Button("PLAY"))
                {
                    EventQueue::Instance().Signal<Event::PlayMode>(true);
                }
            }
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
}

void ImguiWindowMainToolbar::_OnSceneActivated(Event::SceneActivated& e)
{
    const auto& [scene] = e.data_tuple;
    _Scene = scene;
}


void ImguiWindowMainToolbar::_OnSceneDeactivated(Event::SceneDeactivated& e)
{
    _Scene.reset();
}
