#pragma once

#include<memory>

#include"core/app/app_layer.hpp"
#include"function/ui/imgui_layer.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

#include"project/project_events.hpp"
#include"project/project_layer.hpp"

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

    private:
        bool _WaitingForProjectUnbuiltBeforeClosing{ false };
    };

    ARCADIA_API auto CreateApplicationUptr() -> std::unique_ptr<Arcadia::iAppLayer>;
}
