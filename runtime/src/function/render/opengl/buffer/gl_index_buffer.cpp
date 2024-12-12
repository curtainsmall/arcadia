#include "pch.hpp"

#include "gl_index_buffer.hpp"

GlIndexBuffer::GlIndexBuffer(const std::vector<Mesh::index_type>& indices) :
    _IndexCount(indices.size())
{
    ACDA_GL_CALL(glGenBuffers(1, &_GlId));
    Bind();
    ACDA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Mesh::index_type), indices.data(), GL_STATIC_DRAW));
    Unbind();
}

GlIndexBuffer::~GlIndexBuffer()
{
    ACDA_GL_CALL(glDeleteBuffers(1, &_GlId));
}

GlIndexBuffer::GlIndexBuffer(SelfType&& rhs) noexcept
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _IndexCount = rhs._IndexCount;
    rhs._IndexCount = 0;
}

auto GlIndexBuffer::operator=(SelfType&& rhs) noexcept -> SelfType&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _IndexCount = rhs._IndexCount;
    rhs._IndexCount = 0;

    return *this;
}

void GlIndexBuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL index buffer" };
    }

    ACDA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _GlId));
}

void GlIndexBuffer::Unbind() const
{
    ACDA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}