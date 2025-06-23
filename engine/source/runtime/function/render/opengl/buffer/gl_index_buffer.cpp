

#include "gl_index_buffer.hpp"

Arcadia::GlIndexBuffer::GlIndexBuffer(const std::vector<Mesh::IndexType>& indices):
    _IndexCount(indices.size())
{
    ACDA_GL_CALL(glGenBuffers(1, &_GlId));
    Bind();
    ACDA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Mesh::IndexType), indices.data(), GL_STATIC_DRAW));
    Unbind();
}

Arcadia::GlIndexBuffer::~GlIndexBuffer()
{
    ACDA_GL_CALL(glDeleteBuffers(1, &_GlId));
}

Arcadia::GlIndexBuffer::GlIndexBuffer(SelfType&& rhs) noexcept
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _IndexCount = rhs._IndexCount;
    rhs._IndexCount = 0;
}

auto Arcadia::GlIndexBuffer::operator=(SelfType&& rhs) noexcept -> SelfType&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _IndexCount = rhs._IndexCount;
    rhs._IndexCount = 0;

    return *this;
}

void Arcadia::GlIndexBuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw Exceptions::GlInvalid("Cannot bind null OpenGL index buffer");
    }

    ACDA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _GlId));
}

void Arcadia::GlIndexBuffer::Unbind() const
{
    ACDA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}