#pragma once

#include<memory>

#include"core/base.hpp"
#include"resource/scene.hpp"    

#include"project/project_events.hpp"
#include"ui/imgui_window.hpp"

namespace Arcadia
{
    struct ImguiWindowMainToolbar: Arcadia::iImguiWindow
    {
    public:
        using self_type = ImguiWindowMainToolbar;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###toolbar");
    public:
        inline ImguiWindowMainToolbar():
            Arcadia::iImguiWindow(true, "Toolbar")
        {}
        virtual ~ImguiWindowMainToolbar() = default;

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;

    private:
        void _OnSceneActivated(Arcadia::Event::SceneActivated& e);
        void _OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e);
    private:
        std::weak_ptr<Arcadia::Scene> _Scene{};
    };
}
