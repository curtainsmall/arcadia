#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"resource/scene.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_window.hpp"

#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace Arcadia
{
    class ImguiWindowOutliner: public iImguiWindow
    {
    public:
        using SelfType = ImguiWindowOutliner;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###outliner");

        inline ImguiWindowOutliner(
            bool open,
            const std::string& title
        ) :
            iImguiWindow(open, title)
        {}
        virtual ~ImguiWindowOutliner() = default;

        virtual void OnEvent(EventBase& e);
        virtual void OnUpdate();

    private:
        template<cComponent Component>
        void _MenuItemAddComponent(int& item_count);
        template<cComponent Component>
        void _MenuItemRemoveComponent(int& item_count);

        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);
        void _OnSceneActivated(Events::SceneActivated& e);
        void _OnSceneDeactivated(Events::SceneDeactivated& e);
        void _OnRenameEntity(Events::RenameEntity& e);

    private:
        std::weak_ptr<Scene> _Scene{};

        std::string _SelectedEntityName{};

        std::string _EntityOldName{};
        std::string _EntityNewName{};
    };

    template<cComponent Component>
    inline void ImguiWindowOutliner::_MenuItemAddComponent(int& item_count)
    {
        std::string type_str = Component::GetTypeStringStatic();
        bool exists = _Scene.lock()->ContainsAllComponents<Component>(_SelectedEntityName);

        if(!exists)
        {
            ++item_count;
            if(ImGui::MenuItem(type_str.c_str()))
            {
                EventQueue::Instance()
                    .Signal<Events::AddComponent>(_SelectedEntityName, type_str);
            }
        }
    }

    template<cComponent Component>
    inline void ImguiWindowOutliner::_MenuItemRemoveComponent(int& item_count)
    {
        std::string type_str = Component::GetTypeStringStatic();
        bool exists = _Scene.lock()->ContainsAllComponents<Component>(_SelectedEntityName);

        if(exists)
        {
            ++item_count;
            if(exists && ImGui::MenuItem(type_str.c_str()))
            {
                EventQueue::Instance()
                    .Signal<Events::RemoveComponent>(_SelectedEntityName, type_str);
            }
        }
    }
}