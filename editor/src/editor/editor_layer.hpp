#pragma once

#include <memory>

#include "core/app/app_layer.hpp"
#include "function/input/input_events.hpp"
#include "function/window/window_events.hpp"
#include "function/window/window_layer.hpp"

#include "project/project_events.hpp"
#include "project/project_layer.hpp"
#include "ui/imgui_layer.hpp"

namespace Arcadia
{
    struct EditorAppLayer: public AppLayerInterface
    {
    public:
        EditorAppLayer();
        virtual ~EditorAppLayer() = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;
    private:
        void _InstallImguiWindow(ImguiLayer& imgui_layer);
        void _Stop();

        void _OnWindowShouldClose(Events::WindowShouldClose& e);
        void _OnProjectUnbuilt(Events::ProjectUnbuilt& e);
        void _OnWindowCloseCanceled(Events::WindowCloseCanceled& e);
        void _OnTogglePlayMode(Events::TogglePlayMode& e);
        void _OnInputKey(Events::InputKey& e);

    private:
        bool _WaitingForProjectUnbuiltBeforeClosing{ false };
    };

     auto CreateApplicationUnique() -> std::unique_ptr<AppLayerInterface>;
}