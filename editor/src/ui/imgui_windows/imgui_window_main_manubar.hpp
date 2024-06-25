#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"ui/imgui_window.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"

struct ImguiWindowPopupCreateProject
{
public:
    using self_type = ImguiWindowPopupCreateProject;
public:
    void operator()();
public:
    bool open{ false };
private:
    std::string _name{};
    std::string _filepath_str{};
    bool _display_empty_name_warning{ true };
};

struct ImguiWindowPopupCreateScene
{
public:
    using self_type = ImguiWindowPopupCreateScene;
public:
    void operator()(const std::shared_ptr<const Project>& project);
public:
    bool open{ false };
private:
    std::string _name{};
    bool _as_current{ true };
    bool _name_available{ true };
};

struct ImguiWindowMainMenubar: iImguiWindow
{
public:
    using self_type = ImguiWindowMainMenubar;
public:
    ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###menubar");

    inline ImguiWindowMainMenubar(const std::initializer_list<std::tuple<std::string, std::string>>& imgui_window_title_id_pairs):
        iImguiWindow(true, "Main Menubar"),
        _imgui_window_title_and_id_str_pairs(imgui_window_title_id_pairs)
    {}
    virtual ~ImguiWindowMainMenubar() = default;

    virtual void on_event(EventBase& e) override;
    virtual void on_update() override;
private:
    void _file_menu();
    void _edit_menu();
    void _view_menu();
    void _option_menu();

    void _on_project_built(events::ProjectBuilt& e);
    void _on_project_unbuilt(events::ProjectUnbuilt& e);

private:
    ImguiWindowPopupCreateProject _imgui_window_popup_create_project{};
    ImguiWindowPopupCreateScene _imgui_window_popup_create_scene{};

    std::weak_ptr<const Project> _project{};

    std::vector<std::tuple<std::string, std::string>> _imgui_window_title_and_id_str_pairs{};

    bool _show_gizmo{ false };
};