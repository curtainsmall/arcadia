#pragma once

#include<vector>

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/component/model_component/mesh/vertex.hpp"

namespace arcadia
{
    struct ARCADIA_API gl_buffer_layout
    {
    public:
        struct ARCADIA_API attribute
        {
            GLuint count;
            GLenum type;
            GLboolean normalized;
            GLuint divisor;
        };
        using self_type = gl_buffer_layout;
    public:
        gl_buffer_layout() = default;
        ~gl_buffer_layout() = default;

        template<class Type>
        auto push(GLuint count = 1, GLuint dividor = 0) -> self_type&
        {
            ARCADIA_ASSERT(false && "This generic template should never be occured");
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
            stride += arcadia::get_gl_type_size(GL_FLOAT) * count;
            return *this;
        }

        template<>
        auto push<unsigned int>(GLuint count, GLuint divisor) -> self_type&
        {
            layout_attributes.emplace_back(count, GL_UNSIGNED_INT, GL_FALSE, divisor);
            stride += arcadia::get_gl_type_size(GL_UNSIGNED_INT) * count;
            return *this;
        }

        template<>
        auto push<unsigned char>(GLuint count, GLuint divisor) -> self_type&
        {
            layout_attributes.emplace_back(count, GL_UNSIGNED_BYTE, GL_FALSE, divisor);
            stride += arcadia::get_gl_type_size(GL_UNSIGNED_BYTE) * count;
            return *this;
        }
    public:
        std::vector<attribute> layout_attributes{};
        GLsizei stride{};

    };

    struct ARCADIA_API gl_vertex_buffer: arcadia::noncopyable
    {
    public:
        using self_type = gl_vertex_buffer;
    public:
        gl_vertex_buffer(const std::vector<arcadia::vertex>& vertices);
        ~gl_vertex_buffer();

        gl_vertex_buffer(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        inline auto get_id() const -> GLuint
        {
            return _gl_vertex_buffer_id;
        }

        [[nodiscard]]
        inline auto get_vertex_count() const -> GLsizei
        {
            return _vertex_count;
        }

        void bind() const;
        void unbind() const;

        /// @note Call this function @b after the OpenGL vertex array you want to use has been bound
        void setup_vertex_attrib_array() const;
    private:
        GLuint _gl_vertex_buffer_id{ 0 };
        arcadia::gl_buffer_layout _buffer_layout{};
        GLsizei _vertex_count{ 0 };
    };
}
