#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"function/ui/imgui_header.hpp"
#include"function/ui/imgui_window.hpp"
#include"resource/scene/scene.hpp"

#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace Arcadia
{
    struct ARCADIA_API ImguiWindowOutliner: Arcadia::iImguiWindow
    {
    public:
        using self_type = ImguiWindowOutliner;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###outliner");

        inline ImguiWindowOutliner(
            bool open,
            const std::string& title
        ):
            Arcadia::iImguiWindow(open, title)
        {}
        virtual ~ImguiWindowOutliner() = default;

        virtual void OnEvent(Arcadia::EventBase& event);
        virtual void OnUpdate();

    private:
        template<Arcadia::cComponent Component>
        void _MenuItemAddComponent(int& item_count);
        template<Arcadia::cComponent Component>
        void _MenuItemRemoveComponent(int& item_count);

        void _OnOpenImguiWindow(Arcadia::Event::OpenImguiWindow& e);
        void _OnSceneActivated(Arcadia::Event::SceneActivated& e);
        void _OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e);

    private:
        std::shared_ptr<Arcadia::Scene> _Scene{};

        entt::entity _SelectedEntity{ entt::null };

        std::string _EntityOldName{};
        std::string _EntityNewName{};

    };

    template<Arcadia::cComponent Component>
    inline void ImguiWindowOutliner::_MenuItemAddComponent(int& item_count)
    {
        std::string type_str = Component::GetTypeStrStatic();
        bool existed = _Scene->AllOf<Component>(_SelectedEntity);

        if(!existed)
        {
            ++item_count;
            if(ImGui::MenuItem(type_str.c_str()))
            {
                Arcadia::EventQueue::Instance()
                    .Signal<Arcadia::Event::AddComponent>(_SelectedEntity, type_str);
            }
        }
    }

    template<Arcadia::cComponent Component>
    inline void ImguiWindowOutliner::_MenuItemRemoveComponent(int& item_count)
    {

        std::string type_str = Component::GetTypeStrStatic();
        bool existed = _Scene->AllOf<Component>(_SelectedEntity);

        if(existed)
        {
            ++item_count;
            if(existed && ImGui::MenuItem(type_str.c_str()))
            {
                Arcadia::EventQueue::Instance()
                    .Signal<Arcadia::Event::RemoveComponent>(_SelectedEntity, type_str);
            }
        }
    }
}
