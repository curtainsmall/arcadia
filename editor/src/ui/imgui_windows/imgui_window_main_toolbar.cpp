#include "imgui_window_main_toolbar.hpp"

#include"core/event/event.hpp"
#include"core/function.hpp"
#include"core/memento/memento.hpp"
#include"resource/fonts/icon_header.hpp"

#include"editor/editor_context.hpp"
#include"ui/imgui_header.hpp"
#include"ui/ui_events.hpp"

void Arcadia::ImguiWindowMainToolbar::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .IsDispatched();
}

void Arcadia::ImguiWindowMainToolbar::OnUpdate()
{
    std::shared_ptr<Scene> scene_sptr = _SceneWeakPtr.lock();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, glm::vec2{ 0,0 });
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoFocusOnAppearing;
    if(ImGui::BeginViewportSideBar("toolbar", ImGui::GetMainViewport(), ImGuiDir_Up, ImGui::GetFrameHeight(), window_flags))
    {
        MementoList& memento_list = MementoList::Instance();

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
            if(memento_list.GetSize())
            {
                auto iter = memento_list.begin();
                auto end = memento_list.end();
                for(; iter != end; ++iter)
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

        if(scene_sptr)
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
                    EventQueue::Instance().Signal<Events::TogglePlayMode>();
                }
            }
        }

        ImGui::End();
    }
    ImGui::PopStyleVar();
}

void Arcadia::ImguiWindowMainToolbar::_OnSceneActivated(Events::SceneActivated& e)
{
    _SceneWeakPtr = e.Scene;
}

void Arcadia::ImguiWindowMainToolbar::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _SceneWeakPtr.reset();
}