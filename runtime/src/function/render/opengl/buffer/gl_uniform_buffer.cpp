#include "pch.hpp"

#include "gl_uniform_buffer.hpp"

#include"core/log/log.hpp"

GlUniformBuffer::GlUniformBuffer(
    GLsizeiptr size
)
{
    ACDA_GL_CALL(glGenBuffers(1, &_gl_id));

    bind();
    ACDA_GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
    unbind();
}

GlUniformBuffer::GlUniformBuffer(GLsizeiptr size, const GLvoid* data):
    GlUniformBuffer(size)
{
    sub_data(0, size, data);
}

GlUniformBuffer::~GlUniformBuffer()
{
    ACDA_GL_CALL(glDeleteBuffers(1, &_gl_id));
}

GlUniformBuffer::GlUniformBuffer(self_type&& rhs) noexcept:
    _gl_id(rhs._gl_id)
{
    rhs._gl_id = 0;
}

auto GlUniformBuffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;
    return *this;
}

void GlUniformBuffer::bind() const
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL uniform buffer" };
    }

    ACDA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, _gl_id));
}

void GlUniformBuffer::unbind() const
{
    ACDA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void GlUniformBuffer::bind_buffer_base(GLuint index) const
{
    bind();
    ACDA_GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, index, _gl_id));
    unbind();
}

void GlUniformBuffer::bind_buffer_range(GLuint index, GLintptr Offset, GLsizeiptr size) const
{
    bind();
    ACDA_GL_CALL(glBindBufferRange(GL_UNIFORM_BUFFER, index, _gl_id, Offset, size));
    unbind();
}

auto GlUniformBuffer::sub_data(GLintptr offset, GLsizeiptr size, const GLvoid* data) const -> const self_type&
{
    bind();
    ACDA_GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
    unbind();

    return *this;
}
