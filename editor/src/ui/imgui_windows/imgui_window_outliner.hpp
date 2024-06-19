#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_window.hpp"
#include"resource/scene.hpp"

#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

struct ImguiWindowOutliner: iImguiWindow
{
public:
    using self_type = ImguiWindowOutliner;
public:
    ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###outliner");

    inline ImguiWindowOutliner(
        bool open,
        const std::string& title
    ):
        iImguiWindow(open, title)
    {}
    virtual ~ImguiWindowOutliner() = default;

    virtual void on_event(EventBase& e);
    virtual void on_update();

private:
    template<cComponent Component>
    void _menu_item_add_component(int& item_count);
    template<cComponent Component>
    void _menu_item_remove_component(int& item_count);

    void _on_open_imgui_window(event::OpenImguiWindow& e);
    void _on_scene_activated(event::SceneActivated& e);
    void _on_scene_deactivated(event::SceneDeactivated& e);
    void _on_rename_entity(event::RenameEntity& e);

private:
    std::weak_ptr<Scene> _scene{};

    std::string _selected_entity_name{};

    std::string _EntityOldName{};
    std::string _EntityNewName{};

};

template<cComponent Component>
inline void ImguiWindowOutliner::_menu_item_add_component(int& item_count)
{
    std::string type_str = Component::get_type_str_static();
    bool exists = _scene.lock()->all_of<Component>(_selected_entity_name);

    if(!exists)
    {
        ++item_count;
        if(ImGui::MenuItem(type_str.c_str()))
        {
            EventQueue::instance()
                .signal<event::AddComponent>(_selected_entity_name, type_str);
        }
    }
}

template<cComponent Component>
inline void ImguiWindowOutliner::_menu_item_remove_component(int& item_count)
{

    std::string type_str = Component::get_type_str_static();
    bool exists = _scene.lock()->all_of<Component>(_selected_entity_name);

    if(exists)
    {
        ++item_count;
        if(exists && ImGui::MenuItem(type_str.c_str()))
        {
            EventQueue::instance()
                .signal<event::RemoveComponent>(_selected_entity_name, type_str);
        }
    }
}
