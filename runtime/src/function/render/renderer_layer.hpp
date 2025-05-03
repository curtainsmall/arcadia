#pragma once

#include <memory>
#include <filesystem>

#include "core/layer.hpp"
#include "platform/graphic_api.hpp"
#include "function/render/renderer.hpp"

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
        virtual ~RendererLayer();

        virtual void OnEvent(EventBase& event);
        virtual void OnUpdate();

    private:
        std::shared_ptr<RendererInterface> _spRenderer{};
    };
}
