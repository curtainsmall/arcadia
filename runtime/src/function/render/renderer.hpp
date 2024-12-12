#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/math.hpp"
#include"platform/graphic_api/graphic_api.hpp"
#include"resource/scene.hpp"

class RenderResultInfo
{
public:
    void* id;
    glm::ivec2 size;
};

class iRenderer: public Noncopyable
{
public:
    ACDA_EXCEPTION(DrawFail);

    using SelfType = iRenderer;
public:
    iRenderer() = default;
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

    /// @brief Submit entity to draw
    /// @param name Entity name
    /// @note If the entity is not renderable, it is ignored
    virtual void Submit(const Scene& scene, const std::string& name) = 0;

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
    virtual auto GetRenderResultId(size_t index) const->void* = 0;

    /// @brief Get the underlying graphic API type
    virtual auto GetGraphicApiType() const->GraphicApi::Type = 0;
};

template<typename Renderer>
concept cRenderer = requires{
    std::derived_from<Renderer, iRenderer>;
};