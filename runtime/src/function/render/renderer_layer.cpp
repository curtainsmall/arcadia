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

}

Arcadia::RendererLayer::~RendererLayer()
{
}

void Arcadia::RendererLayer::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::RendererSetActive>(ACDA_BIND_MEMBER_FN(_OnRendererSetActive))
        .Dispatch<Events::RendererSetScene>(ACDA_BIND_MEMBER_FN(_OnRendererSetScene))
        .Dispatch<Events::RendererSetEntity>(ACDA_BIND_MEMBER_FN(_OnRendererSetEntity))
        .Dispatch<Events::SceneActivated>(ACDA_BIND_MEMBER_FN(_OnSceneActivated))
        .Dispatch<Events::SceneDeactivated>(ACDA_BIND_MEMBER_FN(_OnSceneDeactivated))
        .IsDispatched();
}

void Arcadia::RendererLayer::OnUpdate()
{
    _spRenderer->Draw();
}

auto Arcadia::RendererLayer::HasRenderer() const -> bool
{
    return !!_spRenderer;
}

auto Arcadia::RendererLayer::HasRenderResult() const -> bool
{
    return _spRenderer->HasRenderResult();
}

auto Arcadia::RendererLayer::GetRenderResultId(EntityId entity_id) const -> void*
{
    return _spRenderer->GetRenderResultId(entity_id);
}

auto Arcadia::RendererLayer::GetCurrentGraphicApiType() const -> GraphicApi::Type
{
    return _spRenderer->GetGraphicApiType();
}

auto Arcadia::RendererLayer::IsRendererActive() const -> bool
{
    return _spRenderer->IsActive();
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

void Arcadia::RendererLayer::_OnSceneActivated(Events::SceneActivated& e)
{
    _spRenderer->SetScene(e.spScene);
}

void Arcadia::RendererLayer::_OnSceneDeactivated(Events::SceneDeactivated& e)
{
    _spRenderer->SetScene(nullptr);
}
