#include "pch.hpp"
#include "gl_index_buffer.hpp"

Arcadia::GlIndexBuffer::GlIndexBuffer(const std::vector<Arcadia::Mesh::index_type>& indices):
    _IndexCount(indices.size())
{
    ARCADIA_GL_CALL(glGenBuffers(1, &_GlId));
    Bind();
    ARCADIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Arcadia::Mesh::index_type), indices.data(), GL_STATIC_DRAW));
    Unbind();
}

Arcadia::GlIndexBuffer::~GlIndexBuffer()
{
    ARCADIA_GL_CALL(glDeleteBuffers(1, &_GlId));
}

Arcadia::GlIndexBuffer::GlIndexBuffer(self_type&& rhs) noexcept
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _IndexCount = rhs._IndexCount;
    rhs._IndexCount = 0;
}

auto Arcadia::GlIndexBuffer::operator=(self_type&& rhs) noexcept -> self_type&
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
        throw Arcadia::GlInvalid{ "Cannot bind null OpenGL index buffer" };
    }

    ARCADIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _GlId));
}

void Arcadia::GlIndexBuffer::Unbind() const
{
    ARCADIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
