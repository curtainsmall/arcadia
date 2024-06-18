#include "pch.hpp"

#include "gl_framebuffer.hpp"

GlFramebuffer::GlFramebuffer(
    const glm::ivec2& viewport_size,
    float near_plane,
    float far_plane
):
    _GlTexture2d(viewport_size),
    _GlDepthStencilRenderbuffer(GL_DEPTH24_STENCIL8, viewport_size)
{
    ACDA_GL_CALL(glGenFramebuffers(1, &_GlId));

    Bind();
    ACDA_GL_CALL(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _GlTexture2d.GetGlId(), 0));
    ACDA_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _GlDepthStencilRenderbuffer.GetGlId()));
    Unbind();

    if(auto res = IsComplete(); res != GL_FRAMEBUFFER_COMPLETE)
    {
        throw GlInvalid{ std::format("OpenGL framebuffer incomplete: {}",res) };
    }
}

GlFramebuffer::~GlFramebuffer()
{
    ACDA_GL_CALL(glDeleteFramebuffers(1, &_GlId));
}

GlFramebuffer::GlFramebuffer(self_type&& rhs) noexcept:
    _GlTexture2d(std::move(rhs._GlTexture2d))
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;
}

auto GlFramebuffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _GlTexture2d = std::move(rhs._GlTexture2d);

    _GlId = rhs._GlId;
    rhs._GlId = 0;

    return *this;
}

void GlFramebuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL framebuffer" };
    }

    ACDA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _GlId));
}

void GlFramebuffer::Unbind() const
{
    ACDA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

auto GlFramebuffer::IsComplete() const -> GLenum
{
    if(_GlId == 0)
    {
        throw GlInvalid{ "Cannot check completeness of null OpenGL framebuffer" };
    }

    Bind();
    ACDA_GL_CALL(auto res = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    Unbind();

    return res;
}
