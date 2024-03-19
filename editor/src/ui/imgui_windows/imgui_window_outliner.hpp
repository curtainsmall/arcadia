#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"function/ui/imgui_header.hpp"
#include"function/ui/imgui_window.hpp"
#include"resource/scene/scene.hpp"

#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_outliner: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_outliner;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###outliner");

        inline imgui_window_outliner(
            bool open,
            const std::string& title
        ):
            imgui_window_interface(open, title)
        {}
        virtual ~imgui_window_outliner() = default;

        virtual void on_event(arcadia::event_base& event);
        virtual void on_update();

    private:
        template<arcadia::component_like Component>
        void _add_component_menu_item(int& item_count);
        template<arcadia::component_like Component>
        void _remove_component_menu_item(int& item_count);

        void _on_open_imgui_window(arcadia::event::open_imgui_window& e);
        void _on_scene_activated(arcadia::event::scene_activated& e);
        void _on_scene_deactivated(arcadia::event::scene_deactivated& e);

    private:
        std::weak_ptr<arcadia::scene> _scene_wptr{};

        entt::entity _selected_entity{ entt::null };

        std::string _entity_old_name{};
        std::string _entity_new_name{};

    };

    template<arcadia::component_like Component>
    inline void imgui_window_outliner::_add_component_menu_item(int& item_count)
    {
        std::shared_ptr<const arcadia::scene> scene_sptr = _scene_wptr.lock();

        std::string type_str = Component::get_type_str_static();
        bool existed = scene_sptr->contains_all_component_of<Component>(_selected_entity);

        if(!existed)
        {
            ++item_count;
            if(ImGui::MenuItem(type_str.c_str()))
            {
                arcadia::event_queue::instance()
                    .signal<arcadia::event::add_component>(_selected_entity, type_str);
            }
        }
    }

    template<arcadia::component_like Component>
    inline void imgui_window_outliner::_remove_component_menu_item(int& item_count)
    {
        std::shared_ptr<const arcadia::scene> scene_sptr = _scene_wptr.lock();

        std::string type_str = Component::get_type_str_static();
        bool existed = scene_sptr->contains_all_component_of<Component>(_selected_entity);

        if(existed)
        {
            ++item_count;
            if(existed && ImGui::MenuItem(type_str.c_str()))
            {
                arcadia::event_queue::instance()
                    .signal<arcadia::event::remove_component>(_selected_entity, type_str);
            }
        }
    }
}
