#include "pch.hpp"
#include "gl_uniform_buffer.hpp"

arcadia::gl_uniform_buffer::gl_uniform_buffer(
    GLsizeiptr size
)
{
    ARCADIA_GL_CALL(glGenBuffers(1, &_gl_id));

    bind();
    ARCADIA_GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
    unbind();
}

arcadia::gl_uniform_buffer::~gl_uniform_buffer()
{
    ARCADIA_GL_CALL(glDeleteBuffers(1, &_gl_id));
}

arcadia::gl_uniform_buffer::gl_uniform_buffer(self_type&& rhs) noexcept:
    _gl_id(rhs._gl_id)
{
    rhs._gl_id = 0;
}

auto arcadia::gl_uniform_buffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;
    return *this;
}

void arcadia::gl_uniform_buffer::bind() const
{
    if(_gl_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot bind null OpenGL uniform buffer" };
    }

    ARCADIA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, _gl_id));
}

void arcadia::gl_uniform_buffer::unbind() const
{
    ARCADIA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void arcadia::gl_uniform_buffer::set_binding(GLuint index)
{
    ARCADIA_GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, index, _gl_id));
}
