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
    inline ImguiWindowMainToolbar():
        iImguiWindow(true, "Toolbar")
    {}
    virtual ~ImguiWindowMainToolbar() = default;

    virtual void OnEvent(EventBase& event) override;
    virtual void OnUpdate() override;

private:
    void _OnSceneActivated(Event::SceneActivated& e);
    void _OnSceneDeactivated(Event::SceneDeactivated& e);
private:
    std::weak_ptr<Scene> _Scene{};
};
