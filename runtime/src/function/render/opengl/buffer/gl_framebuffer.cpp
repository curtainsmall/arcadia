#include "pch.hpp"

#include "gl_framebuffer.hpp"

GlFramebuffer::GlFramebuffer(
    const glm::ivec2& viewport_size,
    float near_plane,
    float far_plane
):
    _gl_texture2d(viewport_size),
    _gl_depth_stencil_renderbuffer(GL_DEPTH24_STENCIL8, viewport_size)
{
    ACDA_GL_CALL(glGenFramebuffers(1, &_gl_id));

    bind();
    ACDA_GL_CALL(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _gl_texture2d.gl_id(), 0));
    ACDA_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _gl_depth_stencil_renderbuffer.gl_id()));
    unbind();

    if(auto res = is_complete(); res != GL_FRAMEBUFFER_COMPLETE)
    {
        throw GlInvalid{ std::format("OpenGL framebuffer incomplete: {}",res) };
    }
}

GlFramebuffer::~GlFramebuffer()
{
    ACDA_GL_CALL(glDeleteFramebuffers(1, &_gl_id));
}

GlFramebuffer::GlFramebuffer(self_type&& rhs) noexcept:
    _gl_texture2d(std::move(rhs._gl_texture2d))
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;
}

auto GlFramebuffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_texture2d = std::move(rhs._gl_texture2d);

    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    return *this;
}

void GlFramebuffer::bind() const
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL framebuffer" };
    }

    ACDA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _gl_id));
}

void GlFramebuffer::unbind() const
{
    ACDA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

auto GlFramebuffer::is_complete() const -> GLenum
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot check completeness of null OpenGL framebuffer" };
    }

    bind();
    ACDA_GL_CALL(auto res = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    unbind();

    return res;
}
