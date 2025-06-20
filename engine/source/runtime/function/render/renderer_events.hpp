#pragma once

#include "core/event.hpp"
#include "function/render/renderer.hpp"

namespace Arcadia::Events
{
    struct ACDA_API RendererReset: public EventBase
    {
    };

    struct ACDA_API RendererSetActive: public EventBase
    {
    public:
        RendererSetActive(bool active):
            Active(active)
        {
        }

    public:
        const bool Active;
    };

    struct ACDA_API RendererSetEntity: public EventBase
    {
    public:
        enum struct ACDA_API ActionType
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