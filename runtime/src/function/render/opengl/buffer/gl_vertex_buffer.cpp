#include "pch.hpp"
#include "gl_vertex_buffer.hpp"

arcadia::gl_vertex_buffer::gl_vertex_buffer(const std::vector<arcadia::vertex>& vertices):
    _vertex_count(vertices.size())
{
    ARCADIA_GL_CALL(glGenBuffers(1, &_gl_vertex_buffer_id));
    bind();
    ARCADIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(arcadia::vertex), vertices.data(), GL_STATIC_DRAW));
    unbind();

    // Setup layout 
    _buffer_layout
        .push<glm::vec3>()  // For coord
        .push<glm::vec3>()  // For normal
        .push<glm::vec2>(); // For tex_coord
}

arcadia::gl_vertex_buffer::~gl_vertex_buffer()
{
    ARCADIA_GL_CALL(glDeleteBuffers(1, &_gl_vertex_buffer_id));
}

arcadia::gl_vertex_buffer::gl_vertex_buffer(self_type&& rhs) noexcept
{
    _gl_vertex_buffer_id = rhs._gl_vertex_buffer_id;
    rhs._gl_vertex_buffer_id = 0;

    _buffer_layout = std::move(rhs._buffer_layout);

    _vertex_count = rhs._vertex_count;
    rhs._vertex_count = 0;
}

auto arcadia::gl_vertex_buffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _gl_vertex_buffer_id = rhs._gl_vertex_buffer_id;
    rhs._gl_vertex_buffer_id = 0;

    _buffer_layout = std::move(rhs._buffer_layout);

    _vertex_count = rhs._vertex_count;
    rhs._vertex_count = 0;

    return *this;
}

void arcadia::gl_vertex_buffer::bind() const
{
    if(_gl_vertex_buffer_id == 0)
    {
        throw arcadia::gl_invalid{ "Cannot bind null OpenGL vertex buffer" };
    }

    ARCADIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _gl_vertex_buffer_id));
}

void arcadia::gl_vertex_buffer::unbind() const
{
    ARCADIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void arcadia::gl_vertex_buffer::setup_vertex_attrib_array() const
{
    GLuint attrib_slot{ 0 };
    std::size_t offset{ 0 };

    bind();
    for(const auto& attribute : _buffer_layout.layout_attributes)
    {
        ARCADIA_GL_CALL(glEnableVertexAttribArray(attrib_slot));
        ARCADIA_GL_CALL(glVertexAttribPointer(attrib_slot, attribute.count, attribute.type, attribute.normalized, _buffer_layout.stride, reinterpret_cast<void*>(offset)));
        ARCADIA_GL_CALL(glVertexAttribDivisor(attrib_slot, attribute.divisor));
        ++attrib_slot;
        offset += attribute.count * arcadia::get_gl_type_size(attribute.type);
    }
    unbind();
}
