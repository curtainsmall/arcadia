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
    auto gl_id() const -> GLuint
    {
        return _gl_id;
    }

    [[nodiscard]]
    auto gl_texure2d() const -> const GlTexture2d&
    {
        return _gl_texture2d;
    }

    void bind() const;
    void unbind() const;

    auto is_complete() const->GLenum;
private:
    GLuint _gl_id{ 0 };
    GlTexture2d _gl_texture2d;
    GlRenderbuffer _gl_depth_stencil_renderbuffer{};
};
