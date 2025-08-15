#pragma once

#include <memory>

#undef ERROR
#include "csys/system.h"

#include "function/window/window_events.hpp"
#include "function/window/window_layer.hpp"

#include "editor/defines.hpp"
#include "project/project_events.hpp"
#include "project/project_layer.hpp"
#include "ui/imgui_layer.hpp"

namespace Arcadia
{
    struct ACDA_API EditorLayer: public LayerInterface
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

        void SetPlayMode(bool play_mode);
        [[nodiscard]]
        auto IsInPlayMode() const -> bool;

        void SetUiScale(float ui_scale);
        [[nodiscard]]
        auto GetUiScale() const -> float;

        [[nodiscard]]
        auto GetCsysSystem() -> csys::System&;

    private:
        void _Stop();

        void _OnWindowShouldClose(Events::WindowShouldClose& e);
        void _OnProjectUnbuilt(Events::ProjectUnbuilt& e);
        void _OnWindowCloseCanceled(Events::WindowCloseCanceled& e);
        void _OnSetPlayMode(Events::SetPlayMode& e);
        void _OnInputKey(Events::InputKey& e);

    private:
        bool _PlayMode{ false };
        float _UiScale{ 1.0f };

        bool _WaitingForProjectUnbuiltBeforeClosing{ false };

        csys::System _CsysSystem{};
    };
}