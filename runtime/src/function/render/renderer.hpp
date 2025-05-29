#pragma once

#include <memory>

#include "core/exception.hpp"
#include "core/math.hpp"
#include "core/noncopyable.hpp"
#include "platform/api_def.hpp"
#include "platform/graphic_api.hpp"
#include "resource/scene.hpp"

namespace Arcadia
{
    struct ACDA_API RenderResultInfo
    {
    public:
        void* Id;
        glm::i32vec2 Size;
    };

    struct ACDA_API RendererInterface: public Noncopyable
    {
    public:

        using SelfType = RendererInterface;
    public:
        RendererInterface() = default;
        virtual ~RendererInterface() = default;

        [[nodiscard]]
        virtual auto HasEntity(EntityId entity_id) const -> bool = 0;
        virtual void BuildEntity(EntityId entity_id) = 0;
        virtual void RemoveEntity(EntityId entity_id) = 0;

        virtual void Draw() = 0;
        virtual void Reset() = 0;

        [[nodiscard]]
        virtual auto HasRenderResult() const -> bool = 0;
        // Render result id type:
        // - OpenGL: GLuint
        // - Direct11: ID3D11ShaderResourceView*
        // - Direct12: D3D12_GPU_DESCRIPTOR_HANDLE
        // - Vulkan: VkDescriptorSet
        [[nodiscard]]
        virtual auto GetRenderResultId(EntityId entity_id) const->void* = 0;
        [[nodiscard]]
        virtual auto GetGraphicApiType() const->GraphicApi::Type = 0;

        [[nodiscard]]
        virtual auto IsActive() const -> bool;
        virtual void SetActive(bool active);
    protected:
        bool _Active{ false };
    };

    namespace Concepts
    {
        template<class T>
        concept Renderer = std::derived_from<T, RendererInterface>;
    }
}