#include "renderer_service.hpp"

#include "core/assert.hpp"
#include "core/event.hpp"
#include "core/function.hpp"
#include "core/match.hpp"
#include "function/render/opengl/gl_renderer.hpp"
#include "function/render/renderer_events.hpp"
#include "resource/scene_service.hpp"

Arcadia::RendererService::RendererService(
    Engine& engine,
    const GraphicApi::Type& graphic_api,
    std::filesystem::path working_directory
):
    _Engine(engine)
{
    _EventSubscription = _Engine.events.SubscribeAny(
        Foundation::Order::Renderer,
        [this](Foundation::Event& e) -> bool
        {
            OnEvent(e);
            return e.Handled();
        }
    );

    MatchVariant<void>(
        graphic_api,
        [&](const GraphicApi::Opengl&)
        {
            static OpenglContext gl_context{};
            _upRenderer = std::make_unique<GlRenderer>(_Engine, working_directory / ToFilepath("shaders/opengl"));
        },
        [](auto&&)
        {
            ACDA_UNREACHABLE("Unknown renderer type");
        }
    );
}

void Arcadia::RendererService::Draw()
{
    OnUpdate();
}

void Arcadia::RendererService::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::RendererReset>(ACDA_BIND_MEMBER_FN(_OnRendererReset))
        .Dispatch<Events::RendererSetActive>(ACDA_BIND_MEMBER_FN(_OnRendererSetActive))
        .Dispatch<Events::RendererSetEntity>(ACDA_BIND_MEMBER_FN(_OnRendererSetEntity))
        .IsDispatched();
}

void Arcadia::RendererService::OnUpdate()
{
    ACDA_ASSERT(_upRenderer);
    _upRenderer->Draw();
}

auto Arcadia::RendererService::HasRenderer() const -> bool
{
    return !!_upRenderer;
}

auto Arcadia::RendererService::RendererHasEntity(EntityId entity_id) const -> bool
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->HasEntity(entity_id);
}

auto Arcadia::RendererService::HasRenderResult() const -> bool
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->HasRenderResult();
}

auto Arcadia::RendererService::GetRenderResultId(EntityId entity_id) const -> void*
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->GetRenderResultId(entity_id);
}

auto Arcadia::RendererService::GetCurrentGraphicApiType() const -> GraphicApi::Type
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->GetGraphicApiType();
}

auto Arcadia::RendererService::IsRendererActive() const -> bool
{
    ACDA_ASSERT(_upRenderer);
    return _upRenderer->IsActive();
}

void Arcadia::RendererService::_OnRendererReset(Events::RendererReset& e)
{
    ACDA_ASSERT(_upRenderer);
    _upRenderer->Reset();
}

void Arcadia::RendererService::_OnRendererSetActive(Events::RendererSetActive& e)
{
    ACDA_ASSERT(_upRenderer);
    _upRenderer->SetActive(e.Active);
}

void Arcadia::RendererService::_OnRendererSetEntity(Events::RendererSetEntity& e)
{
    ACDA_ASSERT(_upRenderer);

    SceneService* scene_service = _Engine.scenes;
    if(!scene_service->HasActiveScene() || !scene_service->ActiveScene_ContainsEntity(e.EntityId))
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
