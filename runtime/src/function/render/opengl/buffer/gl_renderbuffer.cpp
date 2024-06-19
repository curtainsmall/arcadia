#include "pch.hpp"

#include "gl_renderbuffer.hpp"

GlRenderbuffer::GlRenderbuffer()
{
    ACDA_GL_CALL(glGenRenderbuffers(1, &_gl_id));
}

GlRenderbuffer::GlRenderbuffer(GLenum format, const glm::ivec2& size):
    GlRenderbuffer()
{
    set_storage(format, size);
}

GlRenderbuffer::~GlRenderbuffer()
{
    ACDA_GL_CALL(glDeleteRenderbuffers(1, &_gl_id));
}

void GlRenderbuffer::bind() const
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL renderbuffer" };
    }

    ACDA_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, _gl_id));
}

void GlRenderbuffer::unbind() const
{
    ACDA_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void GlRenderbuffer::set_storage(GLenum format, const glm::ivec2& size)
{
    bind();
    ACDA_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, format, size.x, size.y));
    unbind();
}
