#pragma once

#include<vector>

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/mesh/vertex.hpp"

namespace Arcadia
{
    struct ARCADIA_API GlBufferLayout
    {
    public:
        struct ARCADIA_API Attribute
        {
            GLuint count;
            GLenum Type;
            GLboolean normalized;
            GLuint divisor;
        };
        using self_type = GlBufferLayout;
    public:
        GlBufferLayout() = default;
        ~GlBufferLayout() = default;

        template<class Type>
        auto Push(GLuint count = 1, GLuint dividor = 0) -> self_type&
        {
            ARCADIA_ASSERT(false && "This generic template should never be occured");
            return *this;
        }

        template<>
        auto Push<glm::vec4>(GLuint count, GLuint divisor) -> self_type&
        {
            for(GLuint i = 0; i < count; ++i)
            {
                Push<float>(4, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::vec3>(GLuint count, GLuint divisor) -> self_type&
        {
            for(GLuint i = 0; i < count; ++i)
            {
                Push<float>(3, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::vec2>(GLuint count, GLuint divisor) -> self_type&
        {
            for(GLuint i = 0; i < count; ++i)
            {
                Push<float>(2, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::mat4>(GLuint count, GLuint divisor) -> self_type&
        {
            for(GLuint i = 0; i < count; i++)
            {
                Push<glm::vec4>(4, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::mat3>(GLuint count, GLuint divisor) -> self_type&
        {
            for(GLuint i = 0; i < count; i++)
            {
                Push<glm::vec3>(3, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::mat2>(GLuint count, GLuint divisor) -> self_type&
        {
            for(GLuint i = 0; i < count; i++)
            {
                Push<glm::vec2>(2, divisor);
            }
            return *this;
        }

        template<>
        auto Push<float>(GLuint count, GLuint divisor) -> self_type&
        {
            LayoutAttributes.emplace_back(count, GL_FLOAT, GL_FALSE, divisor);
            Stride += Arcadia::GetGlTypeSize(GL_FLOAT) * count;
            return *this;
        }

        template<>
        auto Push<unsigned int>(GLuint count, GLuint divisor) -> self_type&
        {
            LayoutAttributes.emplace_back(count, GL_UNSIGNED_INT, GL_FALSE, divisor);
            Stride += Arcadia::GetGlTypeSize(GL_UNSIGNED_INT) * count;
            return *this;
        }

        template<>
        auto Push<unsigned char>(GLuint count, GLuint divisor) -> self_type&
        {
            LayoutAttributes.emplace_back(count, GL_UNSIGNED_BYTE, GL_FALSE, divisor);
            Stride += Arcadia::GetGlTypeSize(GL_UNSIGNED_BYTE) * count;
            return *this;
        }
    public:
        std::vector<Attribute> LayoutAttributes{};
        GLsizei Stride{};

    };

    struct ARCADIA_API GlVertexBuffer: Arcadia::Noncopyable
    {
    public:
        using self_type = GlVertexBuffer;
    public:
        GlVertexBuffer(const std::vector<Arcadia::Vertex>& vertices);
        ~GlVertexBuffer();

        GlVertexBuffer(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        auto GetGlId() const -> GLuint
        {
            return _GlId;
        }

        [[nodiscard]]
        auto GetVertexCount() const -> GLsizei
        {
            return _VertexCount;
        }

        void Bind() const;
        void Unbind() const;

        /// @note Call this function @b after the OpenGL vertex array you want to use has been bound
        void SetupVertexAttribArray() const;
    private:
        GLuint _GlId{ 0 };
        Arcadia::GlBufferLayout _BufferLayout{};
        GLsizei _VertexCount{ 0 };
    };
}
