#include "pch.hpp"
#include "renderer_layer.hpp"

#include "core/match.hpp"
#include "core/assert.hpp"
#include "core/event.hpp"
#include "function/render/opengl/gl_renderer.hpp"
#include "function/render/renderer_events.hpp"

Arcadia::RendererLayer::RendererLayer(const GraphicApi::Type& graphic_api, std::filesystem::path working_directory):
    LayerInterface("renderer")
{
    MatchVariant<void>(
        graphic_api,
        [&](const GraphicApi::Opengl&)
    {
        static OpenglContext gl_context{};
        _spRenderer = std::make_shared<GlRenderer>(working_directory / ToFilepath("shaders/opengl"));
    },
        [](auto&&)
    {
        ACDA_UNREACHABLE("Unknown renderer type");
    }
    );

    EventQueue::Instance().Signal<Events::RendererBuilt>(_spRenderer);
}

Arcadia::RendererLayer::~RendererLayer()
{
    EventQueue::Instance().Signal<Events::RendererUnbuilt>();
}

void Arcadia::RendererLayer::OnEvent(EventBase& event)
{}

void Arcadia::RendererLayer::OnUpdate()
{}
