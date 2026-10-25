#pragma once

#include <filesystem>
#include <memory>

#include "app/engine.hpp"
#include "app/services.hpp"
#include "function/render/renderer.hpp"
#include "function/render/renderer_events.hpp"
#include "platform/graphic_api.hpp"
#include "resource/scene_events.hpp"

namespace Arcadia
{
    struct ACDA_API RendererService: public Renderer
    {
    public:
        using SelfType = RendererService;
    public:
        RendererService(
            Engine& engine,
            const GraphicApi::Type& graphic_api,
            std::filesystem::path working_directory
        );
        virtual ~RendererService() override = default;

        void Draw() override;

        void OnEvent(EventBase& event);
        void OnUpdate();

        [[nodiscard]]
        auto HasRenderer() const -> bool;

        [[nodiscard]]
        auto RendererHasEntity(EntityId entity_id) const -> bool;

        [[nodiscard]]
        auto HasRenderResult() const -> bool;
        [[nodiscard]]
        auto GetRenderResultId(EntityId entity_id) const -> void*;
        [[nodiscard]]
        auto GetCurrentGraphicApiType() const -> GraphicApi::Type;

        [[nodicard]]
        auto IsRendererActive() const -> bool;

    private:
        void _OnRendererReset(Events::RendererReset& e);

        void _OnRendererSetActive(Events::RendererSetActive& e);
        void _OnRendererSetEntity(Events::RendererSetEntity& e);
    private:
        Engine& _Engine;

        Foundation::Subscription _EventSubscription{};

        std::unique_ptr<RendererInterface> _upRenderer{};
    };
}
