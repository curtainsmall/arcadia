#include "pch.hpp"

#include "gl_uniform_buffer.hpp"

#include"core/log/log.hpp"

GlUniformBuffer::GlUniformBuffer(
    GLsizeiptr size
)
{
    ARCADIA_GL_CALL(glGenBuffers(1, &_GlId));

    Bind();
    ARCADIA_GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
    Unbind();
}

GlUniformBuffer::GlUniformBuffer(GLsizeiptr size, const GLvoid* data):
    GlUniformBuffer(size)
{
    SubData(0, size, data);
}

GlUniformBuffer::~GlUniformBuffer()
{
    ARCADIA_GL_CALL(glDeleteBuffers(1, &_GlId));
}

GlUniformBuffer::GlUniformBuffer(self_type&& rhs) noexcept:
    _GlId(rhs._GlId)
{
    rhs._GlId = 0;
}

auto GlUniformBuffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;
    return *this;
}

void GlUniformBuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL uniform buffer" };
    }

    ARCADIA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, _GlId));
}

void GlUniformBuffer::Unbind() const
{
    ARCADIA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void GlUniformBuffer::BindBufferBase(GLuint index) const
{
    Bind();
    ARCADIA_GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, index, _GlId));
    Unbind();
}

void GlUniformBuffer::BindBufferRange(GLuint index, GLintptr Offset, GLsizeiptr size) const
{
    Bind();
    ARCADIA_GL_CALL(glBindBufferRange(GL_UNIFORM_BUFFER, index, _GlId, Offset, size));
    Unbind();
}

auto GlUniformBuffer::SubData(GLintptr Offset, GLsizeiptr size, const GLvoid* data) const -> const self_type&
{
    Bind();
    ARCADIA_GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, Offset, size, data));
    Unbind();

    return *this;
}
