#include "pch.hpp"

#include "gl_vertex_buffer.hpp"

GlVertexBuffer::GlVertexBuffer(const std::vector<Vertex>& vertices):
    _VertexCount(vertices.size())
{
    ARCADIA_GL_CALL(glGenBuffers(1, &_GlId));
    Bind();
    ARCADIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
    Unbind();

    // Setup layout 
    _BufferLayout
        .Push<glm::vec3>()  // For coord
        .Push<glm::vec3>()  // For normal
        .Push<glm::vec2>(); // For tex_coord
}

GlVertexBuffer::~GlVertexBuffer()
{
    ARCADIA_GL_CALL(glDeleteBuffers(1, &_GlId));
}

GlVertexBuffer::GlVertexBuffer(self_type&& rhs) noexcept
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _BufferLayout = std::move(rhs._BufferLayout);

    _VertexCount = rhs._VertexCount;
    rhs._VertexCount = 0;
}

auto GlVertexBuffer::operator=(self_type&& rhs) noexcept -> self_type&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _BufferLayout = std::move(rhs._BufferLayout);

    _VertexCount = rhs._VertexCount;
    rhs._VertexCount = 0;

    return *this;
}

void GlVertexBuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw GlInvalid{ "Cannot bind null OpenGL vertex buffer" };
    }

    ARCADIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _GlId));
}

void GlVertexBuffer::Unbind() const
{
    ARCADIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GlVertexBuffer::SetupVertexAttribArray() const
{
    GLuint attrib_slot{ 0 };
    std::size_t Offset{ 0 };

    Bind();
    for(const auto& attribute : _BufferLayout.LayoutAttributes)
    {
        ARCADIA_GL_CALL(glEnableVertexAttribArray(attrib_slot));
        ARCADIA_GL_CALL(glVertexAttribPointer(attrib_slot, attribute.Count, attribute.Type, attribute.Normalized, _BufferLayout.Stride, reinterpret_cast<void*>(Offset)));
        ARCADIA_GL_CALL(glVertexAttribDivisor(attrib_slot, attribute.Divisor));
        ++attrib_slot;
        Offset += attribute.Count * GetGlTypeSize(attribute.Type);
    }
    Unbind();
}
