#include "pch.hpp"

#include "gl_uniform_buffer.hpp"

#include "core/log/log.hpp"

Arcadia::GlUniformBuffer::GlUniformBuffer(
    GLsizeiptr size
)
{
    ACDA_GL_CALL(glGenBuffers(1, &_GlId));

    Bind();
    ACDA_GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
    Unbind();
}

Arcadia::GlUniformBuffer::GlUniformBuffer(GLsizeiptr size, const GLvoid* data):
    GlUniformBuffer(size)
{
    SetBufferSubData(0, size, data);
}

Arcadia::GlUniformBuffer::~GlUniformBuffer()
{
    ACDA_GL_CALL(glDeleteBuffers(1, &_GlId));
}

Arcadia::GlUniformBuffer::GlUniformBuffer(SelfType&& rhs) noexcept:
    _GlId(rhs._GlId)
{
    rhs._GlId = 0;
}

auto Arcadia::GlUniformBuffer::operator=(SelfType&& rhs) noexcept -> SelfType&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;
    return *this;
}

void Arcadia::GlUniformBuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw Exceptions::GlInvalid("Cannot bind null OpenGL uniform buffer");
    }

    ACDA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, _GlId));
}

void Arcadia::GlUniformBuffer::Unbind() const
{
    ACDA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void Arcadia::GlUniformBuffer::BindBufferBase(GLuint index) const
{
    Bind();
    ACDA_GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, index, _GlId));
    Unbind();
}

void Arcadia::GlUniformBuffer::BindBufferRange(GLuint index, GLintptr Offset, GLsizeiptr size) const
{
    Bind();
    ACDA_GL_CALL(glBindBufferRange(GL_UNIFORM_BUFFER, index, _GlId, Offset, size));
    Unbind();
}

auto Arcadia::GlUniformBuffer::SetBufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data) const -> const SelfType&
{
    Bind();
    ACDA_GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
    Unbind();

    return *this;
}