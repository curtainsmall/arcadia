#pragma once

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/math.hpp"
#include"platform/graphic_api/graphic_api.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/light_component/light_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/physics_component/physics_component.hpp"
#include"resource/component/skybox_component/skybox_component.hpp"

namespace Arcadia
{
    struct ARCADIA_API RenderResultInfo
    {
    public:
        void* id;
        glm::ivec2 size;
    };

    struct ARCADIA_API iRenderer: Arcadia::Noncopyable
    {
    public:
        ARCADIA_EXCEPTION(DrawFail);

        using self_type = iRenderer;
    public:
        virtual ~iRenderer() = default;

        /// @brief Check whether the physcis simulator is in build
        virtual auto IsInBuild() const -> bool = 0;

        /// @brief Start building the renderer
        /// @details This function signs that the renderer is in build
        /// @note This function can only be called when the renderer is not in build
        virtual void Prepare() = 0;

        /// @brief Finish building the renderer
        /// @details This function signs that current frame is complete and ready to draw
        /// @note This function can only be called when the renderer is in build
        virtual void Finalize() = 0;

        /// @brief Submit a camera component to this renderer
        /// @note This function can only be called when the renderer is in build
        virtual void Submit(const Arcadia::CameraComponent&) = 0;

        /// @brief Submit a light component to this renderer
        /// @note This function can only be called when the renderer is in build
        virtual void Submit(const Arcadia::LightComponent&) = 0;

        /// @brief Submit a mesh component to this renderer
        /// @note This function can only be called when the renderer is in build
        virtual void Submit(const Arcadia::ModelComponent&) = 0;

        /// @brief Submit a skybox component to this renderer
        /// @note This function can only be called when the renderer is in build
        virtual void Submit(const Arcadia::SkyboxComponent&) = 0;

        /// @brief Submit a physics component to this renderer
        /// @details This function is used to render shape of the body of the physics component; If there is no body, do nothing
        /// @note This function can only be called when the renderer is in build
        virtual void Submit(const Arcadia::PhysicsComponent&) = 0;

        /// @brief Draw curtain frame
        /// @throw draw_fail if the draw call failed for any reason
        /// @note This function can only be called when the renderer is not in build
        virtual void Draw() = 0;

        /// @brief Reset the renderer, all caches will be cleared
        virtual void Reset() = 0;

        /// @brief Get the render result (the framebuffer) id
        /// @details Object id type for graphic APIs:
        /// - OpenGL: GLuint
        /// - DirectX11: ID3D11ShaderResourceView*
        /// - DirectX12: D3D12_GPU_DESCRIPTOR_HANDLE
        /// - Vulkan: VkDescriptorSet
        /// @param index Index of framebuffer
        /// @return Representation of framebuffer object id as void*
        virtual auto GetRenderResultId(std::size_t index) const->void* = 0;

        /// @brief Get the underlying graphic API type
        virtual auto GetGraphicApiType() const->Arcadia::GraphicApi::Type = 0;

    };

    template<class Renderer>
    concept cRenderer = requires{
        std::derived_from<Renderer, Arcadia::iRenderer>;
    };

}
