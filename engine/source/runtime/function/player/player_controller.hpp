#pragma once

#include "platform/defines.hpp"
#include "app/engine.hpp"
#include "resource/entity_id.hpp"

namespace Arcadia
{
    struct ACDA_API PlayerController
    {
    public:
        using SelfType = PlayerController;
    public:
        explicit PlayerController(Engine& engine);
        ~PlayerController() = default;

        PlayerController(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        [[nodiscard]]
        auto IsActive() const -> bool;
        void SetActive(bool active);

        [[nodiscard]]
        auto GetActiveEntityId() const -> EntityId;
        void SetActiveEntityId(EntityId entity_id);

        void ProcessInputKey(GlfwInputKey key, GlfwInputAction action, GlfwInputModifier mod);
        void ProcessInputMouseButton(GlfwInputMouseButton mouse_button, GlfwInputAction action, GlfwInputModifier mod);
    private:
        Engine& _Engine;

        bool _Active{ false };
        EntityId _ActiveEntityId{};
    };
}