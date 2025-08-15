#pragma once

#include "core/event.hpp"

namespace Arcadia::Events
{
    struct ACDA_API PlayerControllerSetActive: EventBase
    {
    public:
        PlayerControllerSetActive(bool active):
            Active(active)
        {
        }
    public:
        const bool Active;
    };

    struct ACDA_API PlayerControllerSetEntity: EventBase
    {
    public:
        PlayerControllerSetEntity(EntityId entity_id):
            EntityId(entity_id)
        {
        }

    public:
        EntityId EntityId;
    };
}