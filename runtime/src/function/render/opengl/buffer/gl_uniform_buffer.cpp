#include "pch.hpp"

#include "gl_uniform_buffer.hpp"

#include"core/log/log.hpp"

GlUniformBuffer::GlUniformBuffer(
    GLsizeiptr size
)
{
    ACDA_GL_CALL(glGenBuffers(1, &_GlId));

    Bind();
    ACDA_GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
    Unbind();
}

GlUniformBuffer::GlUniformBuffer(GLsizeiptr size, const GLvoid* data) :
    GlUniformBuffer(size)
{
    SetBufferSubData(0, size, data);
}

GlUniformBuffer::~GlUniformBuffer()
{
    ACDA_GL_CALL(glDeleteBuffers(1, &_GlId));
}

GlUniformBuffer::GlUniformBuffer(SelfType&& rhs) noexcept :
    _GlId(rhs._GlId)
{
    rhs._GlId = 0;
}

auto GlUniformBuffer::operator=(SelfType&& rhs) noexcept -> SelfType&
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

    ACDA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, _GlId));
}

void GlUniformBuffer::Unbind() const
{
    ACDA_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void GlUniformBuffer::BindBufferBase(GLuint index) const
{
    Bind();
    ACDA_GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, index, _GlId));
    Unbind();
}

void GlUniformBuffer::BindBufferRange(GLuint index, GLintptr Offset, GLsizeiptr size) const
{
    Bind();
    ACDA_GL_CALL(glBindBufferRange(GL_UNIFORM_BUFFER, index, _GlId, Offset, size));
    Unbind();
}

auto GlUniformBuffer::SetBufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data) const -> const SelfType&
{
    Bind();
    ACDA_GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
    Unbind();

    return *this;
}