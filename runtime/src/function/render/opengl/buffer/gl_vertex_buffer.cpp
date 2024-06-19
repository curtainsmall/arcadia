#include "pch.hpp"

#include "gl_vertex_buffer.hpp"

GlVertexBuffer::GlVertexBuffer(const std::vector<Vertex>& vertices):
    _vertex_count(vertices.size())
{
    ACDA_GL_CALL(glGenBuffers(1, &_gl_id));
    bind();
    ACDA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
    unbind();

    // Setup layout 
    _buffer_layout
        .push<glm::vec3>()  // For coord
        .push<glm::vec3>()  // For normal
        .push<glm::vec2>(); // For tex_coord
}

GlVertexBuffer::~GlVertexBuffer()
{
    ACDA_GL_CALL(glDeleteBuffers(1, &_gl_id));
}

GlVertexBuffer::GlVertexBuffer(self_type&& rhs) noexcept
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    _buffer_layout = std::move(rhs._buffer_layout);

    _vertex_count = rhs._vertex_count;
    rhs._vertex_count = 0;
}

auto GlVertexBuffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_id = rhs._gl_id;
    rhs._gl_id = 0;

    _buffer_layout = std::move(rhs._buffer_layout);

    _vertex_count = rhs._vertex_count;
    rhs._vertex_count = 0;

    return *this;
}

void GlVertexBuffer::bind() const
{
    if(_gl_id == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL vertex buffer" };
    }

    ACDA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _gl_id));
}

void GlVertexBuffer::unbind() const
{
    ACDA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GlVertexBuffer::setup_vertex_attrib_array() const
{
    GLuint attrib_slot{ 0 };
    std::size_t Offset{ 0 };

    bind();
    for(const auto& attribute : _buffer_layout.layout_attributes)
    {
        ACDA_GL_CALL(glEnableVertexAttribArray(attrib_slot));
        ACDA_GL_CALL(glVertexAttribPointer(attrib_slot, attribute.count, attribute.type, attribute.normalized, _buffer_layout.stride, reinterpret_cast<void*>(Offset)));
        ACDA_GL_CALL(glVertexAttribDivisor(attrib_slot, attribute.divisor));
        ++attrib_slot;
        Offset += attribute.count * gl_sizeof(attribute.type);
    }
    unbind();
}
