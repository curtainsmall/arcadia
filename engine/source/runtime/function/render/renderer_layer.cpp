#include "renderer_layer.hpp"

#include "core/assert.hpp"
#include "core/event.hpp"
#include "core/function.hpp"
#include "core/match.hpp"
#include "function/render/opengl/gl_renderer.hpp"
#include "function/render/renderer_events.hpp"
#include "resource/scene_layer.hpp"

Arcadia::RendererLayer::RendererLayer(const GraphicApi::Type& graphic_api, std::filesystem::path working_directory):
    LayerInterface("renderer")
{
    MatchVariant<void>(
        graphic_api,
        [&](const GraphicApi::Opengl&)
        {
            static OpenglContext gl_context{};
            _upRenderer = std::make_unique<GlRenderer>(working_directory / ToFilepath("shaders/opengl"));
        },
        [](auto&&)
        {
            ACDA_UNREACHABLE("Unknown renderer type");
        }
    );
}

void Arcadia::RendererLayer::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::RendererReset>(ACDA_BIND_MEMBER_FN(_OnRendererReset))
        .Dispatch<Events::RendererSetActive>(ACDA_BIND_MEMBER_FN(_OnRendererSetActive))
        .Dispatch<Events::RendererSetEntity>(ACDA_BIND_MEMBER_FN(_OnRendererSetEntity))
        .IsDispatched();
}

void Arcadia::RendererLayer::OnUpdate()
{
    ACDA_ASSERT(_upRenderer);
    _upRenderer->Draw();
}

auto Arcadia::RendererLayer::HasRenderer() const -> bool
{
    return !!_upRenderer;
}

auto Arcadia::RendererLayer::RendererHasEntity(EntityId entity_id) const -> bool
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->HasEntity(entity_id);
}

auto Arcadia::RendererLayer::HasRenderResult() const -> bool
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->HasRenderResult();
}

auto Arcadia::RendererLayer::GetRenderResultId(EntityId entity_id) const -> void*
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->GetRenderResultId(entity_id);
}

auto Arcadia::RendererLayer::GetCurrentGraphicApiType() const -> GraphicApi::Type
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->GetGraphicApiType();
}

auto Arcadia::RendererLayer::IsRendererActive() const -> bool
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->IsActive();
}

void Arcadia::RendererLayer::_OnRendererReset(Events::RendererReset& e)
{
    ACDA_ASSERT(_upRenderer);
    _upRenderer->Reset();
}

void Arcadia::RendererLayer::_OnRendererSetActive(Events::RendererSetActive& e)
{
    ACDA_ASSERT(_upRenderer);
    _upRenderer->SetActive(e.Active);
}

void Arcadia::RendererLayer::_OnRendererSetEntity(Events::RendererSetEntity& e)
{
    ACDA_ASSERT(_upRenderer);

    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
    if(!scene_layer_sptr->HasActiveScene() || !scene_layer_sptr->ActiveScene_ContainsEntity(e.EntityId))
    {
        return;
    }

    switch(e.ActionType)
    {
        case Events::RendererSetEntity::ActionType::Build:
        {
            _upRenderer->BuildEntity(e.EntityId);
            break;
        }
        case Events::RendererSetEntity::ActionType::Remove:
        {
            _upRenderer->RemoveEntity(e.EntityId);
            break;
        }
        default:
        {
            ACDA_UNREACHABLE("Invalid action type");
        }
    }
}