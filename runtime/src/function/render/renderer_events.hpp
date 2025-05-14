#pragma once

#include "core/event.hpp"
#include "function/render/renderer.hpp"

namespace Arcadia::Events
{
    class RendererReset: public EventBase
    {
    };

    class RendererSetActive: public EventBase
    {
    public:
        RendererSetActive(bool active):
            Active(active)
        {
        }

    public:
        const bool Active;
    };

    class RendererSetEntity: public EventBase
    {
    public:
        enum class ActionType
        {
            Add,
            Remove,
            Update,
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