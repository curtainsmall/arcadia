#include "pch.hpp"
#include "gl_framebuffer.hpp"

arcadia::gl_framebuffer::gl_framebuffer(
    const glm::ivec2& viewport_size,
    float near_plane,
    float far_plane
):
    _gl_texture2d(viewport_size),
    _gl_depth_stencil_renderbuffer(GL_DEPTH24_STENCIL8, viewport_size)
{
    ARCADIA_GL_CALL(glGenFramebuffers(1, &_gl_id));

    bind();
    ARCADIA_GL_CALL(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _gl_texture2d.get_gl_id(), 0));
    ARCADIA_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _gl_depth_stencil_renderbuffer.get_gl_id()));
    unbind();

    if(auto res = is_complete(); res != GL_FRAMEBUFFER_COMPLETE)
    {
        throw arcadia::gl_invalid{ std::format("OpenGL framebuffer incomplete: {}",res) };
    }
}

arcadia::gl_framebuffer::~gl_framebuffer()
{
    ARCADIA_GL_CALL(glDeleteFramebuffers(1, &_gl_id));
}

arcadia::gl_framebuffer::gl_framebuffer(self_type&& rhs) noexcept:
    _gl_texture2d(std::move(rhs._gl_texture2d))
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;
}

auto arcadia::gl_framebuffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_texture2d = std::move(rhs._gl_texture2d);

    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    return *this;
}

void arcadia::gl_framebuffer::bind() const
{
    if(_gl_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot bind null OpenGL framebuffer" };
    }

    ARCADIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _gl_id));
}

void arcadia::gl_framebuffer::unbind() const
{
    ARCADIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

auto arcadia::gl_framebuffer::is_complete() const -> GLenum
{
    if(_gl_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot check completeness of null OpenGL framebuffer" };
    }

    bind();
    ARCADIA_GL_CALL(auto res = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    unbind();

    return res;
}
