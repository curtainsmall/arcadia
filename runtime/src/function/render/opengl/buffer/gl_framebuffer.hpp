#pragma once

#include"core/base.hpp"
#include"core/exception.hpp"
#include"function/render/opengl/buffer/gl_renderbuffer.hpp"
#include"function/render/opengl/buffer/gl_texture2d.hpp"
#include"platform/opengl/opengl_header.hpp"

struct GlFramebuffer:Noncopyable
{
public:
    ACDA_EXCEPTION(Imcomplete);

    using self_type = GlFramebuffer;
public:
    GlFramebuffer(
        const glm::ivec2& viewport_size,
        float near_plane,
        float far_plane
    );
    ~GlFramebuffer();

    GlFramebuffer(self_type&& rhs) noexcept;
    auto operator=(self_type&& rhs) noexcept -> self_type&;

    [[nodiscard]]
    auto GetGlID() const -> GLuint
    {
        return _GlId;
    }

    [[nodiscard]]
    auto GetGlTexture2d() const -> const GlTexture2d&
    {
        return _GlTexture2d;
    }

    void Bind() const;
    void Unbind() const;

    auto IsComplete() const->GLenum;
private:
    GLuint _GlId{ 0 };
    GlTexture2d _GlTexture2d;
    GlRenderbuffer _GlDepthStencilRenderbuffer{};
};
