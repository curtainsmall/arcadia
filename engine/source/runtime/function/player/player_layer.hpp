#pragma once

#include "platform/defines.hpp"
#include "core/layer.hpp"
#include "function/player/player_controller.hpp"
#include "function/player/player_events.hpp"
#include "function/window/window_events.hpp"

namespace Arcadia
{
    struct ACDA_API PlayerLayer: LayerInterface
    {
    public:
        using SelfType = PlayerLayer;
    public:
        PlayerLayer() = default;
        virtual ~PlayerLayer() override = default;

        virtual void OnEvent(EventBase& event) override;
        virtual void OnUpdate() override;

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

        void _ProcessInputKey(GlfwInputKey key, GlfwInputAction action, GlfwInputModifier mod);
        void _ProcessInputMouseButton(GlfwInputMouseButton mouse_button, GlfwInputAction action, GlfwInputModifier mod);
    private:
        PlayerController _PlayerController{};
    };
}