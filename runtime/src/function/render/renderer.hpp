#pragma once

#include<memory>

#include"core/exception.hpp"
#include"core/math.hpp"
#include"core/noncopyable.hpp"
#include"platform/api_def.hpp"
#include"platform/graphic_api/graphic_api.hpp"
#include"resource/scene.hpp"

namespace Arcadia
{
    class RenderResultInfo
    {
    public:
        void* id;
        glm::i32vec2 size;
    };

    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(RendererDrawFail);
    }

    class RendererInterface: public Noncopyable
    {
    public:

        using SelfType = RendererInterface;
    public:
        RendererInterface() = default;
        virtual ~RendererInterface() = default;

        virtual auto IsInBuild() const -> bool = 0;

        virtual void Prepare() = 0;

        virtual void Finalize() = 0;

        virtual void Submit(const Scene& scene, EntityId entity_id) = 0;

        virtual void Draw() = 0;

        virtual void Reset() = 0;

        // Render result id type:
        // - OpenGL: GLuint
        // - Direct11: ID3D11ShaderResourceView*
        // - Direct12: D3D12_GPU_DESCRIPTOR_HANDLE
        // - Vulkan: VkDescriptorSet
        virtual auto GetRenderResultId(std::size_t index) const->void* = 0;

        virtual auto GetGraphicApiType() const->GraphicApi::Type = 0;
    };

    namespace Concepts
    {
        template<typename T>
        concept Renderer = std::derived_from<T, RendererInterface>;
    }
}