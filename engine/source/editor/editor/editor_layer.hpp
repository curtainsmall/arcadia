#pragma once

#include <memory>

#include "function/input/input_events.hpp"
#include "function/window/window_events.hpp"
#include "function/window/window_layer.hpp"

#include "project/project_events.hpp"
#include "project/project_layer.hpp"
#include "ui/imgui_layer.hpp"

namespace Arcadia
{
    struct EditorLayer: public LayerInterface
    {
    public:
        EditorLayer() = default;
        virtual ~EditorLayer() = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

        void SetPlayMode(bool play_mode);
        auto GetPlayMode() const -> bool;

        void SetUiScale(float ui_scale);
        auto GetUiScale() const -> float;

    private:
        void _Stop();

        void _OnWindowShouldClose(Events::WindowShouldClose& e);
        void _OnProjectUnbuilt(Events::ProjectUnbuilt& e);
        void _OnWindowCloseCanceled(Events::WindowCloseCanceled& e);
        void _OnTogglePlayMode(Events::TogglePlayMode& e);
        void _OnInputKey(Events::InputKey& e);

    private:
        bool _PlayMode{ false };
        float _UiScale{ -1.0f };

        bool _WaitingForProjectUnbuiltBeforeClosing{ false };
    };
}