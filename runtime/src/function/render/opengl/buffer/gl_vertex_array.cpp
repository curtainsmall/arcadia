#include "pch.hpp"
#include "gl_vertex_array.hpp"

arcadia::gl_vertex_array::gl_vertex_array(
    const std::vector<arcadia::vertex>& vertices,
    const std::vector<arcadia::mesh::index_type>& indices
):
    _gl_vertex_buffer(vertices),
    _gl_index_buffer(indices)
{
    ARCADIA_GL_CALL(glGenVertexArrays(1, &_gl_id));
    bind();
    _gl_vertex_buffer.setup_vertex_attrib_array();
    unbind();
}

arcadia::gl_vertex_array::~gl_vertex_array()
{
    ARCADIA_GL_CALL(glDeleteVertexArrays(1, &_gl_id));
}

arcadia::gl_vertex_array::gl_vertex_array(self_type&& rhs) noexcept:
    _gl_vertex_buffer(std::move(rhs._gl_vertex_buffer)),
    _gl_index_buffer(std::move(rhs._gl_index_buffer))
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;
}

auto arcadia::gl_vertex_array::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_vertex_buffer = std::move(rhs._gl_vertex_buffer);
    _gl_index_buffer = std::move(rhs._gl_index_buffer);

    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    return *this;
}

void arcadia::gl_vertex_array::bind() const
{
    if(_gl_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot bind null OpenGL vertex array" };
    }

    ARCADIA_GL_CALL(glBindVertexArray(_gl_id));

    _gl_vertex_buffer.bind();
    _gl_index_buffer.bind();
}

void arcadia::gl_vertex_array::unbind() const
{
    _gl_index_buffer.unbind();
    _gl_vertex_buffer.unbind();

    ARCADIA_GL_CALL(glBindVertexArray(0));
}

void arcadia::gl_vertex_array::draw(GLenum mode, GLsizei count) const
{
    if(_gl_index_buffer.get_index_count())
    {
        if(count < 0)
        {
            draw_indices(mode);
        }
        else
        {
            draw_indices_instanced(mode, count);
        }
    }
    else
    {
        if(count < 0)
        {
            draw_arrays(mode);
        }
        else
        {
            draw_arrays_instanced(mode, count);
        }
    }
}

void arcadia::gl_vertex_array::draw_arrays(GLenum mode) const
{
    ARCADIA_GL_CALL(glDrawArrays(mode, 0, _gl_vertex_buffer.get_vertex_count()));
}

void arcadia::gl_vertex_array::draw_arrays_instanced(GLenum mode, GLsizei count) const
{
    ARCADIA_GL_CALL(glDrawArraysInstanced(mode, 0, _gl_vertex_buffer.get_vertex_count(), count));
}

void arcadia::gl_vertex_array::draw_indices(GLenum mode) const
{
    ARCADIA_GL_CALL(glDrawElements(mode, _gl_index_buffer.get_index_count(), GL_UNSIGNED_INT, 0));
}

void arcadia::gl_vertex_array::draw_indices_instanced(GLenum mode, GLsizei count) const
{
    ARCADIA_GL_CALL(glDrawElementsInstanced(mode, _gl_index_buffer.get_index_count(), GL_UNSIGNED_INT, 0, count));
}
