#pragma once

#include <memory>

#undef ERROR
#include "csys/system.h"

#include "app/app.hpp"
#include "app/engine.hpp"
#include "function/window/window_events.hpp"
#include "function/window/window_service.hpp"

#include "editor/defines.hpp"
#include "project/project_events.hpp"
#include "project/project_service.hpp"
#include "ui/ui_service.hpp"

namespace Arcadia
{
    struct ACDA_API EditorApp: public App
    {
    public:
        EditorApp(Engine& engine);
        virtual ~EditorApp() override;

        void OnFrame(Engine& engine) override;
        void OnStop(Engine& engine) override;

        void OnEvent(EventBase& e);
        void OnUpdate();

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

        void _CollectConfig();

        void _OnWindowShouldClose(Events::WindowShouldClose& e);
        void _OnProjectUnbuilt(Events::ProjectUnbuilt& e);
        void _OnWindowCloseCanceled(Events::WindowCloseCanceled& e);
        void _OnSetPlayMode(Events::SetPlayMode& e);
        void _OnInputKey(Events::InputKey& e);

    private:
        Engine& _Engine;

        Foundation::Subscription _EventSubscription{};

        // The editor composes its own services; they are torn down with it.
        std::unique_ptr<ProjectService> _upProject{};
        std::unique_ptr<UiService> _upUi{};

        bool _PlayMode{ false };
        float _UiScale{ 1.0f };

        bool _WaitingForProjectUnbuiltBeforeClosing{ false };

        csys::System _CsysSystem{};
    };
}
