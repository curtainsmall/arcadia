#include "pch.hpp"

#include "gl_index_buffer.hpp"

GlIndexBuffer::GlIndexBuffer(const std::vector<Mesh::index_type>& indices):
    _index_count(indices.size())
{
    ACDA_GL_CALL(glGenBuffers(1, &_gl_id));
    bind();
    ACDA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Mesh::index_type), indices.data(), GL_STATIC_DRAW));
    unbind();
}

GlIndexBuffer::~GlIndexBuffer()
{
    ACDA_GL_CALL(glDeleteBuffers(1, &_gl_id));
}

GlIndexBuffer::GlIndexBuffer(self_type&& rhs) noexcept
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    _index_count = rhs._index_count;
    rhs._index_count = 0;
}

auto GlIndexBuffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    _index_count = rhs._index_count;
    rhs._index_count = 0;

    return *this;
}

void GlIndexBuffer::bind() const
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL index buffer" };
    }

    ACDA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _gl_id));
}

void GlIndexBuffer::unbind() const
{
    ACDA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
