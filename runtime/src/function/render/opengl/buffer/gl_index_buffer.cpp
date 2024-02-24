#include "pch.hpp"
#include "gl_index_buffer.hpp"

arcadia::gl_index_buffer::gl_index_buffer(const std::vector<arcadia::mesh_component::index_type>& indices):
    _index_count(indices.size())
{
    ARCADIA_GL_CALL(glGenBuffers(1, &_gl_index_buffer_id));
    bind();
    ARCADIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(arcadia::mesh_component::index_type), indices.data(), GL_STATIC_DRAW));
    unbind();
}

arcadia::gl_index_buffer::~gl_index_buffer()
{
    ARCADIA_GL_CALL(glDeleteBuffers(1, &_gl_index_buffer_id));
}

arcadia::gl_index_buffer::gl_index_buffer(self_type&& rhs) noexcept
{
    _gl_index_buffer_id = rhs._gl_index_buffer_id;
    rhs._gl_index_buffer_id = 0;

    _index_count = rhs._index_count;
    rhs._index_count = 0;
}

auto arcadia::gl_index_buffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_index_buffer_id = rhs._gl_index_buffer_id;
    rhs._gl_index_buffer_id = 0;

    _index_count = rhs._index_count;
    rhs._index_count = 0;

    return *this;
}

void arcadia::gl_index_buffer::bind() const
{
    if(_gl_index_buffer_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot bind null OpenGL index buffer" };
    }

    ARCADIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _gl_index_buffer_id));
}

void arcadia::gl_index_buffer::unbind() const
{
    ARCADIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
