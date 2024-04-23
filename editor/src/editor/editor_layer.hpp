#pragma once

#include<memory>

#include"core/app/app_layer.hpp"
#include"function/input/input_events.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

#include"project/project_events.hpp"
#include"project/project_layer.hpp"
#include"ui/imgui_layer.hpp"

namespace Arcadia
{
    struct ARCADIA_API EditorAppLayer: Arcadia::iAppLayer
    {
    public:
        EditorAppLayer();
        virtual ~EditorAppLayer() = default;

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;
    private:
        void _ImguiWindowInstaller(Arcadia::ImguiLayer& imgui_layer);
        void _Stop();

        void _OnWindowShouldClose(Arcadia::Event::WindowShouldClose& e);
        void _OnProjectUnbuilt(Arcadia::Event::ProjectUnbuilt& e);
        void _OnWindowCloseCanceled(Arcadia::Event::WindowCloseCanceled& e);
        void _OnPlayMode(Arcadia::Event::PlayMode& e);
        void _OnInputKey(Arcadia::Event::InputKey& e);

    private:
        bool _WaitingForProjectUnbuiltBeforeClosing{ false };
    };

    ARCADIA_API auto CreateApplication() -> std::unique_ptr<Arcadia::iAppLayer>;
}
