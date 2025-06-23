

#include "gl_renderbuffer.hpp"

Arcadia::GlRenderbuffer::GlRenderbuffer()
{
    ACDA_GL_CALL(glGenRenderbuffers(1, &_GlId));
}

Arcadia::GlRenderbuffer::GlRenderbuffer(GLenum format, const glm::i32vec2& size):
    GlRenderbuffer()
{
    SetStorage(format, size);
}

Arcadia::GlRenderbuffer::~GlRenderbuffer()
{
    ACDA_GL_CALL(glDeleteRenderbuffers(1, &_GlId));
}

void Arcadia::GlRenderbuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw Exceptions::GlInvalid("Cannot bind null OpenGL renderbuffer");
    }

    ACDA_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, _GlId));
}

void Arcadia::GlRenderbuffer::Unbind() const
{
    ACDA_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void Arcadia::GlRenderbuffer::SetStorage(GLenum format, const glm::i32vec2& size)
{
    Bind();
    ACDA_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, format, size.x, size.y));
    Unbind();
}