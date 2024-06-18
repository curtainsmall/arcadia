#include "pch.hpp"

#include "gl_renderbuffer.hpp"

GlRenderbuffer::GlRenderbuffer()
{
    ARCADIA_GL_CALL(glGenRenderbuffers(1, &_GlId));
}

GlRenderbuffer::GlRenderbuffer(GLenum format, const glm::ivec2& size):
    GlRenderbuffer()
{
    SetStorage(format, size);
}

GlRenderbuffer::~GlRenderbuffer()
{
    ARCADIA_GL_CALL(glDeleteRenderbuffers(1, &_GlId));
}

void GlRenderbuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL renderbuffer" };
    }

    ARCADIA_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, _GlId));
}

void GlRenderbuffer::Unbind() const
{
    ARCADIA_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void GlRenderbuffer::SetStorage(GLenum format, const glm::ivec2& size)
{
    Bind();
    ARCADIA_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, format, size.x, size.y));
    Unbind();
}
