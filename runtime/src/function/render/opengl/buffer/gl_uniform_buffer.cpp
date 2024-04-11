#include "pch.hpp"
#include "gl_uniform_buffer.hpp"

#include"core/log/log.hpp"

Arcadia::GlUniformBuffer::GlUniformBuffer(
    GLsizeiptr size
)
{
    ARCADIA_GL_CALL(glGenBuffers(1, &_GlId));

    Bind();
    ARCADIA_GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
    Unbind();
}

Arcadia::GlUniformBuffer::GlUniformBuffer(GLsizeiptr size, const GLvoid* data):
    GlUniformBuffer(size)
{
    SubData(0, size, data);
}

Arcadia::GlUniformBuffer::~GlUniformBuffer()
{
    ARCADIA_GL_CALL(glDeleteBuffers(1, &_GlId));
}

Arcadia::GlUniformBuffer::GlUniformBuffer(self_type&& rhs) noexcept:
    _GlId(rhs._GlId)
{
    rhs._GlId = 0;
}

auto Arcadia::GlUniformBuffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;
    return *this;
}

void Arcadia::GlUniformBuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw Arcadia::GlInvalid{ "Cannot bind null OpenGL uniform buffer" };
    }

    ARCADIA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, _GlId));
}

void Arcadia::GlUniformBuffer::Unbind() const
{
    ARCADIA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void Arcadia::GlUniformBuffer::BindBufferBase(GLuint index) const
{
    Bind();
    ARCADIA_GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, index, _GlId));
    Unbind();
}

void Arcadia::GlUniformBuffer::BindBufferRange(GLuint index, GLintptr Offset, GLsizeiptr size) const
{
    Bind();
    ARCADIA_GL_CALL(glBindBufferRange(GL_UNIFORM_BUFFER, index, _GlId, Offset, size));
    Unbind();
}

auto Arcadia::GlUniformBuffer::SubData(GLintptr Offset, GLsizeiptr size, const GLvoid* data) const -> const self_type&
{
    Bind();
    ARCADIA_GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, Offset, size, data));
    Unbind();

    return *this;
}
