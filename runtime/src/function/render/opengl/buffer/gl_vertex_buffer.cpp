#include "pch.hpp"

#include "gl_vertex_buffer.hpp"

Arcadia::GlVertexBuffer::GlVertexBuffer(const std::vector<Vertex>& vertices):
    _VertexCount(vertices.size())
{
    ACDA_GL_CALL(glGenBuffers(1, &_GlId));
    Bind();
    ACDA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
    Unbind();

    // Setup layout
    _BufferLayout
        .Push<glm::vec3>()  // For coord
        .Push<glm::vec3>()  // For normal
        .Push<glm::vec2>(); // For tex_coord
}

Arcadia::GlVertexBuffer::~GlVertexBuffer()
{
    ACDA_GL_CALL(glDeleteBuffers(1, &_GlId));
}

Arcadia::GlVertexBuffer::GlVertexBuffer(SelfType&& rhs) noexcept
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _BufferLayout = std::move(rhs._BufferLayout);

    _VertexCount = rhs._VertexCount;
    rhs._VertexCount = 0;
}

auto Arcadia::GlVertexBuffer::operator=(SelfType&& rhs) noexcept -> SelfType&
{
    _GlId = rhs._GlId;
    rhs._GlId = 0;

    _BufferLayout = std::move(rhs._BufferLayout);

    _VertexCount = rhs._VertexCount;
    rhs._VertexCount = 0;

    return *this;
}

void Arcadia::GlVertexBuffer::Bind() const
{
    if(_GlId == 0)
    {
        throw Exceptions::GlInvalid("Cannot bind null OpenGL vertex buffer");
    }

    ACDA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _GlId));
}

void Arcadia::GlVertexBuffer::Unbind() const
{
    ACDA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Arcadia::GlVertexBuffer::SetupVertexAttributeArray() const
{
    GLuint attrib_slot{ 0 };
    std::size_t Offset{ 0 };

    Bind();
    for(const GlBufferLayoutAttribute& attribute : _BufferLayout.LayoutAttributes)
    {
        ACDA_GL_CALL(glEnableVertexAttribArray(attrib_slot));
        ACDA_GL_CALL(glVertexAttribPointer(attrib_slot, attribute.Count, attribute.Type, attribute.Normalized, _BufferLayout.Stride, reinterpret_cast<void*>(Offset)));
        ACDA_GL_CALL(glVertexAttribDivisor(attrib_slot, attribute.Divisor));
        ++attrib_slot;
        Offset += attribute.Count * GlGetTypeSize(attribute.Type);
    }
    Unbind();
}