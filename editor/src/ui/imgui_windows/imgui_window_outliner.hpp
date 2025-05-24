#pragma once

#include <memory>
#include <string>

#include "platform/api_def.hpp"
#include "resource/scene_events.hpp"
#include "resource/scene_layer.hpp"

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
            bool open,
            const std::string& title
        );
        virtual ~ImguiWindowOutliner() override = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

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
        std::string type_string = Component::GetTypeStringStatic();
        bool exists = LayerStack::Instance().GetLayerShared<SceneLayer>()->ActiveScene_ContainsAllComponents<Component>(_SelectedEntityId);

        if(!exists)
        {
            ++item_count;
            if(ImGui::MenuItem(type_string.c_str()))
            {
                EventQueue::Instance()
                    .Signal<Events::AddComponent>(_SelectedEntityId, type_string);
            }
        }
    }

    template<Concepts::Component Component>
    void ImguiWindowOutliner::_MenuItemRemoveComponent(int& item_count)
    {
        std::string type_string = Component::GetTypeStringStatic();
        bool exists = LayerStack::Instance().GetLayerShared<SceneLayer>()->ActiveScene_ContainsAllComponents<Component>(_SelectedEntityId);

        if(exists)
        {
            ++item_count;
            if(exists && ImGui::MenuItem(type_string.c_str()))
            {
                EventQueue::Instance()
                    .Signal<Events::RemoveComponent>(_SelectedEntityId, type_string);
            }
        }
    }
}