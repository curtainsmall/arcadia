#pragma once

#include "core/event.hpp"
#include "function/render/renderer.hpp"

namespace Arcadia::Events
{
    class RendererBuilt: public EventBase
    {
    public:
        RendererBuilt(std::shared_ptr<RendererInterface> renderer_sptr):
            spRenderer(renderer_sptr)
        {
        }
    public:
        const std::shared_ptr<RendererInterface> spRenderer;
    };

    class RendererUnbuilt: public EventBase
    {
    };

    class RendererSetActive:public EventBase
    {
    public:
        RendererSetActive(bool active):
            Active(active)
        {
        }

    public:
        const bool Active;
    };

    class RendererSetScene: public EventBase
    {
    public:
        RendererSetScene(const std::shared_ptr<Scene>& scene_sptr):
            spScene(scene_sptr)
        {
        }
    public:
        std::shared_ptr<Scene> spScene;
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