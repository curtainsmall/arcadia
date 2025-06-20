#pragma once

#include <filesystem>
#include <memory>

#include "core/layer.hpp"
#include "function/render/renderer.hpp"
#include "function/render/renderer_events.hpp"
#include "platform/graphic_api.hpp"
#include "resource/scene_events.hpp"

namespace Arcadia
{
    struct ACDA_API RendererLayer: public LayerInterface
    {
    public:
        using SelfType = RendererLayer;
    public:
        RendererLayer(
            const GraphicApi::Type& graphic_api,
            std::filesystem::path working_directory
        );
        virtual ~RendererLayer() override = default;

        virtual void OnEvent(EventBase& event) override;
        virtual void OnUpdate() override;

        [[nodiscard]]
        auto HasRenderer() const -> bool;

        [[nodiscard]]
        auto RendererHasEntity(EntityId entity_id) const -> bool;

        [[nodiscard]]
        auto HasRenderResult() const -> bool;
        [[nodiscard]]
        auto GetRenderResultId(EntityId entity_id) const -> void*;
        [[nodiscard]]
        auto GetCurrentGraphicApiType() const->GraphicApi::Type;

        [[nodicard]]
        auto IsRendererActive() const -> bool;

    private:
        void _OnRendererReset(Events::RendererReset& e);

        void _OnRendererSetActive(Events::RendererSetActive& e);
        void _OnRendererSetEntity(Events::RendererSetEntity& e);
    private:
        std::shared_ptr<RendererInterface> _spRenderer{};
    };
}