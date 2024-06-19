#pragma once

#include<vector>

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/mesh/vertex.hpp"

struct GlBufferLayout
{
public:
    struct Attribute
    {
        GLuint count;
        GLenum type;
        GLboolean normalized;
        GLuint divisor;
    };
    using self_type = GlBufferLayout;
public:
    GlBufferLayout() = default;
    ~GlBufferLayout() = default;

    template<class Type>
    auto push(GLuint count = 1, GLuint dividor = 0) -> self_type&
    {
        ACDA_ASSERT(false && "This generic template should never be occured");
        return *this;
    }

    template<>
    auto push<glm::vec4>(GLuint count, GLuint divisor) -> self_type&
    {
        for(GLuint i = 0; i < count; ++i)
        {
            push<float>(4, divisor);
        }
        return *this;
    }

    template<>
    auto push<glm::vec3>(GLuint count, GLuint divisor) -> self_type&
    {
        for(GLuint i = 0; i < count; ++i)
        {
            push<float>(3, divisor);
        }
        return *this;
    }

    template<>
    auto push<glm::vec2>(GLuint count, GLuint divisor) -> self_type&
    {
        for(GLuint i = 0; i < count; ++i)
        {
            push<float>(2, divisor);
        }
        return *this;
    }

    template<>
    auto push<glm::mat4>(GLuint count, GLuint divisor) -> self_type&
    {
        for(GLuint i = 0; i < count; i++)
        {
            push<glm::vec4>(4, divisor);
        }
        return *this;
    }

    template<>
    auto push<glm::mat3>(GLuint count, GLuint divisor) -> self_type&
    {
        for(GLuint i = 0; i < count; i++)
        {
            push<glm::vec3>(3, divisor);
        }
        return *this;
    }

    template<>
    auto push<glm::mat2>(GLuint count, GLuint divisor) -> self_type&
    {
        for(GLuint i = 0; i < count; i++)
        {
            push<glm::vec2>(2, divisor);
        }
        return *this;
    }

    template<>
    auto push<float>(GLuint count, GLuint divisor) -> self_type&
    {
        layout_attributes.emplace_back(count, GL_FLOAT, GL_FALSE, divisor);
        stride += gl_sizeof(GL_FLOAT) * count;
        return *this;
    }

    template<>
    auto push<unsigned int>(GLuint count, GLuint divisor) -> self_type&
    {
        layout_attributes.emplace_back(count, GL_UNSIGNED_INT, GL_FALSE, divisor);
        stride += gl_sizeof(GL_UNSIGNED_INT) * count;
        return *this;
    }

    template<>
    auto push<unsigned char>(GLuint count, GLuint divisor) -> self_type&
    {
        layout_attributes.emplace_back(count, GL_UNSIGNED_BYTE, GL_FALSE, divisor);
        stride += gl_sizeof(GL_UNSIGNED_BYTE) * count;
        return *this;
    }
public:
    std::vector<Attribute> layout_attributes{};
    GLsizei stride{};

};

struct GlVertexBuffer: Noncopyable
{
public:
    using self_type = GlVertexBuffer;
public:
    GlVertexBuffer(const std::vector<Vertex>& vertices);
    ~GlVertexBuffer();

    GlVertexBuffer(self_type&& rhs) noexcept;
    auto operator=(self_type&& rhs) noexcept -> self_type&;

    [[nodiscard]]
    auto gl_id() const -> GLuint
    {
        return _gl_id;
    }

    [[nodiscard]]
    auto vertex_count() const -> GLsizei
    {
        return _vertex_count;
    }

    void bind() const;
    void unbind() const;

    /// @note Call this function @b after the OpenGL vertex array you want to use has been bound
    void setup_vertex_attrib_array() const;
private:
    GLuint _gl_id{ 0 };
    GlBufferLayout _buffer_layout{};
    GLsizei _vertex_count{ 0 };
};
