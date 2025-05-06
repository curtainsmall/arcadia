#pragma once

#include <filesystem>
#include <memory>

#include "core/layer.hpp"
#include "function/render/renderer.hpp"
#include "function/render/renderer_events.hpp"
#include "platform/graphic_api.hpp"

namespace Arcadia
{
    class RendererLayer: public LayerInterface
    {
    public:
        using SelfType = RendererLayer;
    public:
        RendererLayer(
            const GraphicApi::Type& graphic_api,
            std::filesystem::path working_directory
        );
        virtual ~RendererLayer() override;

        virtual void OnEvent(EventBase& event) override;
        virtual void OnUpdate() override;

    private:
        void _OnRendererSetActive(Events::RendererSetActive& e);
        void _OnRendererSetScene(Events::RendererSetScene& e);
        void _OnRendererSetEntity(Events::RendererSetEntity& e);
    private:
        std::shared_ptr<RendererInterface> _spRenderer{};
    };
}