#pragma once

#include "platform/defines.hpp"
#include "app/engine.hpp"
#include "app/services.hpp"
#include "function/player/player_controller.hpp"
#include "function/player/player_events.hpp"
#include "function/window/window_events.hpp"

namespace Arcadia
{
    struct ACDA_API PlayerService: public Players
    {
    public:
        using SelfType = PlayerService;
    public:
        PlayerService(Engine& engine);
        virtual ~PlayerService() override = default;

        void Update() override;

        void OnEvent(EventBase& event);
        void OnUpdate();

        [[nodiscard]]
        auto IsPlayerControllerActive() const -> bool;

        [[nodiscard]]
        auto GetActiveEntityId() const -> EntityId;

    private:
        void _OnPlayerControllerSetActive(Events::PlayerControllerSetActive& e);
        void _OnPlayerControllerSetEntity(Events::PlayerControllerSetEntity& e);
        void _OnInputKey(Events::InputKey& e);
        void _OnInputCursorMove(Events::InputCursorMove& e);
        void _OnInputScroll(Events::InputScroll& e);
        void _OnInputMouseButton(Events::InputMouseButton& e);

    private:
        Engine& _Engine;

        Foundation::Subscription _EventSubscription{};

        PlayerController _PlayerController;
    };
}
