#include "pch.hpp"
#include "gl_vertex_array.hpp"

Arcadia::GlVertexArray::GlVertexArray(
    const std::vector<Arcadia::Vertex>& vertices,
    const std::vector<Arcadia::Mesh::index_type>& indices
):
    _GlVertexBuffer(vertices),
    _GlIndexBuffer(indices)
{
    ARCADIA_GL_CALL(glGenVertexArrays(1, &_GlId));
    Bind();
    _GlVertexBuffer.SetupVertexAttribArray();
    Unbind();
}

Arcadia::GlVertexArray::~GlVertexArray()
{
    ARCADIA_GL_CALL(glDeleteVertexArrays(1, &_GlId));
}

Arcadia::GlVertexArray::GlVertexArray(self_type&& rhs) noexcept:
    _GlVertexBuffer(std::move(rhs._GlVertexBuffer)),
    _GlIndexBuffer(std::move(rhs._GlIndexBuffer))
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;
}

auto Arcadia::GlVertexArray::operator=(self_type&& rhs) noexcept -> self_type&
{
    _GlVertexBuffer = std::move(rhs._GlVertexBuffer);
    _GlIndexBuffer = std::move(rhs._GlIndexBuffer);

    _GlId = rhs._GlId;
    rhs._GlId = 0;

    return *this;
}

void Arcadia::GlVertexArray::Bind() const
{
    if(_GlId == 0)
    {
        throw Arcadia::GlInvalid{ "Cannot bind null OpenGL vertex array" };
    }

    ARCADIA_GL_CALL(glBindVertexArray(_GlId));

    _GlVertexBuffer.Bind();
    _GlIndexBuffer.Bind();
}

void Arcadia::GlVertexArray::Unbind() const
{
    _GlIndexBuffer.Unbind();
    _GlVertexBuffer.Unbind();

    ARCADIA_GL_CALL(glBindVertexArray(0));
}

void Arcadia::GlVertexArray::Draw(GLenum mode, GLsizei count) const
{
    if(_GlIndexBuffer.GetIndexCount())
    {
        if(count < 0)
        {
            DrawIndices(mode);
        }
        else
        {
            DrawIndicesInstanced(mode, count);
        }
    }
    else
    {
        if(count < 0)
        {
            DrawArrays(mode);
        }
        else
        {
            DrawArraysInstanced(mode, count);
        }
    }
}

void Arcadia::GlVertexArray::DrawArrays(GLenum mode) const
{
    ARCADIA_GL_CALL(glDrawArrays(mode, 0, _GlVertexBuffer.GetVertexCount()));
}

void Arcadia::GlVertexArray::DrawArraysInstanced(GLenum mode, GLsizei count) const
{
    ARCADIA_GL_CALL(glDrawArraysInstanced(mode, 0, _GlVertexBuffer.GetVertexCount(), count));
}

void Arcadia::GlVertexArray::DrawIndices(GLenum mode) const
{
    ARCADIA_GL_CALL(glDrawElements(mode, _GlIndexBuffer.GetIndexCount(), GL_UNSIGNED_INT, 0));
}

void Arcadia::GlVertexArray::DrawIndicesInstanced(GLenum mode, GLsizei count) const
{
    ARCADIA_GL_CALL(glDrawElementsInstanced(mode, _GlIndexBuffer.GetIndexCount(), GL_UNSIGNED_INT, 0, count));
}
