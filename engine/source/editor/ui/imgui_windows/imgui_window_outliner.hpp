#pragma once

#include <memory>
#include <string>

#include "resource/scene_events.hpp"
#include "resource/scene_service.hpp"

#include "project/project_events.hpp"
#include "ui/imgui.hpp"
#include "ui/imgui_window.hpp"
#include "ui/ui_events.hpp"

namespace Arcadia
{
    struct ImguiWindowOutliner: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowOutliner;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###outliner");

        ImguiWindowOutliner(
            Engine& engine,
            bool open,
            std::string_view title
        );
        virtual ~ImguiWindowOutliner() override = default;

        void OnEvent(EventBase& e);
        void OnUpdate();

    private:
        template<Concepts::Component Component>
        void _MenuItemAddComponent(int& item_count);
        template<Concepts::Component Component>
        void _MenuItemRemoveComponent(int& item_count);

        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);
        void _OnSceneDeactivated(Events::SceneDeactivated& e);

    private:
        EntityId _SelectedEntityId{};

        std::string _EntityOldName{};
        std::string _EntityNewName{};
    };

    template<Concepts::Component Component>
    void ImguiWindowOutliner::_MenuItemAddComponent(int& item_count)
    {
        std::string_view type_string = Component::GetTypeStringStatic();
        bool exists = _Engine.scenes->ActiveScene_ContainsAllComponents<Component>(_SelectedEntityId);

        if(!exists)
        {
            ++item_count;
            if(ImGui::MenuItem(type_string.data()))
            {
                _Engine.events
                    .Signal<Events::AddComponent>(_SelectedEntityId, type_string);
            }
        }
    }

    template<Concepts::Component Component>
    void ImguiWindowOutliner::_MenuItemRemoveComponent(int& item_count)
    {
        std::string_view type_string = Component::GetTypeStringStatic();
        bool exists = _Engine.scenes->ActiveScene_ContainsAllComponents<Component>(_SelectedEntityId);

        if(exists)
        {
            ++item_count;
            if(exists && ImGui::MenuItem(type_string.data()))
            {
                _Engine.events
                    .Signal<Events::RemoveComponent>(_SelectedEntityId, type_string);
            }
        }
    }
}