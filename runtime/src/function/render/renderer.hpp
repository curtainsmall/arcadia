#pragma once

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/math.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/skybox_component/skybox_component.hpp"

namespace arcadia
{
    struct ARCADIA_API render_result_info
    {
    public:
        void* id;
        glm::ivec2 size;
    };

    struct ARCADIA_API renderer_interface: arcadia::noncopyable
    {
    public:
        ARCADIA_EXCEPTION(frame_in_build);
        ARCADIA_EXCEPTION(frame_not_in_build);
        ARCADIA_EXCEPTION(draw_fail);

        using self_type = renderer_interface;
    public:
        virtual ~renderer_interface() = default;

        /// @brief Begin a new frame
        /// @details This function signs that a new frame is started to form, any data in the previous frame may be erased (see submit() functions)
        /// @throw frame_in_build if there is already a frame in build
        virtual void begin_frame() = 0;

        /// @brief End curtain frame
        /// @details This function signs that current frame is complete and ready to draw
        /// @throw frame_not_in_build if there is no frame in build
        virtual void end_frame() = 0;

        /// @brief Submit a camera component to this renderer
        /// @note If this function is not called in a new frame, data from previous frame is used
        /// @throw frame_not_in_build if there is no frame in build
        virtual void submit(const arcadia::camera_component&) = 0;

        /// @brief Submit a mesh component to this renderer
        /// @note If this function is not called in a new frame, data from previous frame is used
        /// @throw frame_not_in_build if there is no frame in build
        virtual void submit(const arcadia::model_component&) = 0;

        /// @brief Submit a skybox component to this renderer
        /// @note This function replaces existing skybox since only one skybox can be rendered at a time
        /// @throw frame_not_in_build if there is no frame in build
        virtual void submit(const arcadia::skybox_component&) = 0;

        /// @brief Draw curtain frame
        /// @throw frame_in_build if the frame is in build
        /// @throw draw_fail if the draw call failed for any reason
        virtual void draw() = 0;

        /// @brief Clear all caches
        virtual void clear() = 0;

        /// @brief Get the render result (the framebuffer) id
        /// @details Object id type for graphic apis:
        /// - OpenGL: GLuint
        /// - DirectX11: ID3D11ShaderResourceView*
        /// - DirectX12: D3D12_GPU_DESCRIPTOR_HANDLE
        /// - Vulkan: VkDescriptorSet
        /// @param index Index of framebuffer
        /// @return Representation of framebuffer object id as void*
        virtual auto get_render_result_id(std::size_t index) const->void* = 0;

    };

    template<class Renderer>
    concept renderer_like = requires{
        std::derived_from<Renderer, arcadia::renderer_interface>;
    };

}
