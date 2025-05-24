#pragma once

#include "core/event.hpp"
#include "function/render/renderer.hpp"

namespace Arcadia::Events
{
    struct RendererReset: public EventBase
    {
    };

    struct RendererSetActive: public EventBase
    {
    public:
        RendererSetActive(bool active):
            Active(active)
        {
        }

    public:
        const bool Active;
    };

    struct RendererSetEntity: public EventBase
    {
    public:
        enum struct ActionType
        {
            Build,
            Remove,
        };
    public:
        RendererSetEntity(EntityId entitiy_id, ActionType active_type):
            EntityId(entitiy_id),
            ActionType(active_type)
        {
        }
    public:
        EntityId EntityId;
        ActionType ActionType;
    };
}