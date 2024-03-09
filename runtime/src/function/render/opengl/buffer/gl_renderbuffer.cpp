#include "pch.hpp"
#include "gl_renderbuffer.hpp"

arcadia::gl_renderbuffer::gl_renderbuffer()
{
    ARCADIA_GL_CALL(glGenRenderbuffers(1, &_gl_id));
}

arcadia::gl_renderbuffer::gl_renderbuffer(GLenum format, const glm::ivec2& size):
    gl_renderbuffer()
{
    set_storage(format, size);
}

arcadia::gl_renderbuffer::~gl_renderbuffer()
{
    ARCADIA_GL_CALL(glDeleteRenderbuffers(1, &_gl_id));
}

void arcadia::gl_renderbuffer::bind() const
{
    if(_gl_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot bind null OpenGL renderbuffer" };
    }

    ARCADIA_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, _gl_id));
}

void arcadia::gl_renderbuffer::unbind() const
{
    ARCADIA_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void arcadia::gl_renderbuffer::set_storage(GLenum format, const glm::ivec2& size)
{
    bind();
    ARCADIA_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, format, size.x, size.y));
    unbind();
}
