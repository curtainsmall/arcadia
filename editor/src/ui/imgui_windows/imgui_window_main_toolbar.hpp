#pragma once

#include<memory>

#include"core/base.hpp"
#include"resource/scene.hpp"

#include"project/project_events.hpp"
#include"ui/imgui_window.hpp"

struct ImguiWindowMainToolbar: iImguiWindow
{
public:
    using self_type = ImguiWindowMainToolbar;
public:
    ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###toolbar");
public:
    ImguiWindowMainToolbar() :
        iImguiWindow(true, "Toolbar")
    {}
    virtual ~ImguiWindowMainToolbar() = default;

    virtual void on_event(EventBase& e) override;
    virtual void on_update() override;

private:
    void _on_scene_activated(events::SceneActivated& e);
    void _on_scene_deactivated(events::SceneDeactivated& e);
private:
    std::weak_ptr<Scene> _scene{};
};