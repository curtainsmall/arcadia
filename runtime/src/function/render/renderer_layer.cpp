#include "pch.hpp"
#include "renderer_layer.hpp"

#include "core/assert.hpp"
#include "core/event.hpp"
#include "core/function.hpp"
#include "core/match.hpp"
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
{
    EventDispatcher(event)
        .Dispatch<Events::RendererSetActive>(ACDA_BIND_MEMBER_FN(_OnRendererSetActive))
        .Dispatch<Events::RendererSetScene>(ACDA_BIND_MEMBER_FN(_OnRendererSetScene))
        .Dispatch<Events::RendererSetEntity>(ACDA_BIND_MEMBER_FN(_OnRendererSetEntity))
        .IsDispatched();
}

void Arcadia::RendererLayer::OnUpdate()
{
    _spRenderer->Draw();
}

void Arcadia::RendererLayer::_OnRendererSetActive(Events::RendererSetActive& e)
{
    _spRenderer->SetActive(e.Active);
}

void Arcadia::RendererLayer::_OnRendererSetScene(Events::RendererSetScene& e)
{
    _spRenderer->SetScene(e.spScene);
}

void Arcadia::RendererLayer::_OnRendererSetEntity(Events::RendererSetEntity& e)
{
    switch(e.ActionType)
    {
        case Events::RendererSetEntity::ActionType::Add:
        {
            _spRenderer->AddEntity(e.EntityId);
            break;
        }
        case Events::RendererSetEntity::ActionType::Remove:
        {
            _spRenderer->RemoveEntity(e.EntityId);
            break;
        }
        case Events::RendererSetEntity::ActionType::Update:
        {
            _spRenderer->UpdateEntity(e.EntityId);
            break;
        }
        default:
        {
            ACDA_UNREACHABLE("Invalid action type");
        }
    }
}