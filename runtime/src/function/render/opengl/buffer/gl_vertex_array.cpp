#include "pch.hpp"

#include "gl_vertex_array.hpp"

GlVertexArray::GlVertexArray(
    const std::vector<Vertex>& vertices,
    const std::vector<Mesh::index_type>& indices
):
    _gl_vertex_buffer(vertices),
    _gl_index_buffer(indices)
{
    ACDA_GL_CALL(glGenVertexArrays(1, &_gl_id));
    bind();
    _gl_vertex_buffer.setup_vertex_attrib_array();
    unbind();
}

GlVertexArray::~GlVertexArray()
{
    ACDA_GL_CALL(glDeleteVertexArrays(1, &_gl_id));
}

GlVertexArray::GlVertexArray(self_type&& rhs) noexcept:
    _gl_vertex_buffer(std::move(rhs._gl_vertex_buffer)),
    _gl_index_buffer(std::move(rhs._gl_index_buffer))
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;
}

auto GlVertexArray::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_vertex_buffer = std::move(rhs._gl_vertex_buffer);
    _gl_index_buffer = std::move(rhs._gl_index_buffer);

    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    return *this;
}

void GlVertexArray::bind() const
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL vertex array" };
    }

    ACDA_GL_CALL(glBindVertexArray(_gl_id));

    _gl_vertex_buffer.bind();
    _gl_index_buffer.bind();
}

void GlVertexArray::unbind() const
{
    _gl_index_buffer.unbind();
    _gl_vertex_buffer.unbind();

    ACDA_GL_CALL(glBindVertexArray(0));
}

void GlVertexArray::draw(GLenum mode, GLsizei count) const
{
    if(_gl_index_buffer.index_count())
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

void GlVertexArray::draw_arrays(GLenum mode) const
{
    ACDA_GL_CALL(glDrawArrays(mode, 0, _gl_vertex_buffer.vertex_count()));
}

void GlVertexArray::draw_arrays_instanced(GLenum mode, GLsizei count) const
{
    ACDA_GL_CALL(glDrawArraysInstanced(mode, 0, _gl_vertex_buffer.vertex_count(), count));
}

void GlVertexArray::draw_indices(GLenum mode) const
{
    ACDA_GL_CALL(glDrawElements(mode, _gl_index_buffer.index_count(), GL_UNSIGNED_INT, 0));
}

void GlVertexArray::draw_indices_instanced(GLenum mode, GLsizei count) const
{
    ACDA_GL_CALL(glDrawElementsInstanced(mode, _gl_index_buffer.index_count(), GL_UNSIGNED_INT, 0, count));
}
