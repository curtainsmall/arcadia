#pragma once

#include <vector>

#include "core/assert.hpp"
#include "core/math.hpp"
#include "core/noncopyable.hpp"
#include "platform/api_def.hpp"
#include "platform/opengl.hpp"
#include "resource/mesh/vertex.hpp"

namespace Arcadia
{
    class GlBufferLayoutAttribute
    {
    public:
        GLuint Count;
        GLenum Type;
        GLboolean Normalized;
        GLuint Divisor;
    };

    class GlBufferLayout
    {
    public:
        using SelfType = GlBufferLayout;
    public:
        GlBufferLayout() = default;
        ~GlBufferLayout() = default;

        template<typename Type>
        auto Push(GLuint count = 1, GLuint dividor = 0) -> SelfType&
        {
            ACDA_UNREACHABLE("This generic template should never be occured");
            return *this;
        }

        template<>
        auto Push<glm::vec4>(GLuint count, GLuint divisor) -> SelfType&
        {
            for(GLuint i = 0; i < count; ++i)
            {
                Push<float>(4, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::vec3>(GLuint count, GLuint divisor) -> SelfType&
        {
            for(GLuint i = 0; i < count; ++i)
            {
                Push<float>(3, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::vec2>(GLuint count, GLuint divisor) -> SelfType&
        {
            for(GLuint i = 0; i < count; ++i)
            {
                Push<float>(2, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::mat4>(GLuint count, GLuint divisor) -> SelfType&
        {
            for(GLuint i = 0; i < count; i++)
            {
                Push<glm::vec4>(4, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::mat3>(GLuint count, GLuint divisor) -> SelfType&
        {
            for(GLuint i = 0; i < count; i++)
            {
                Push<glm::vec3>(3, divisor);
            }
            return *this;
        }

        template<>
        auto Push<glm::mat2>(GLuint count, GLuint divisor) -> SelfType&
        {
            for(GLuint i = 0; i < count; i++)
            {
                Push<glm::vec2>(2, divisor);
            }
            return *this;
        }

        template<>
        auto Push<float>(GLuint count, GLuint divisor) -> SelfType&
        {
            LayoutAttributes.emplace_back(count, GL_FLOAT, GL_FALSE, divisor);
            Stride += GlGetTypeSize(GL_FLOAT) * count;
            return *this;
        }

        template<>
        auto Push<unsigned int>(GLuint count, GLuint divisor) -> SelfType&
        {
            LayoutAttributes.emplace_back(count, GL_UNSIGNED_INT, GL_FALSE, divisor);
            Stride += GlGetTypeSize(GL_UNSIGNED_INT) * count;
            return *this;
        }

        template<>
        auto Push<unsigned char>(GLuint count, GLuint divisor) -> SelfType&
        {
            LayoutAttributes.emplace_back(count, GL_UNSIGNED_BYTE, GL_FALSE, divisor);
            Stride += GlGetTypeSize(GL_UNSIGNED_BYTE) * count;
            return *this;
        }
    public:
        std::vector<GlBufferLayoutAttribute> LayoutAttributes{};
        GLsizei Stride{};
    };

    class GlVertexBuffer: public Noncopyable
    {
    public:
        using SelfType = GlVertexBuffer;
    public:
        GlVertexBuffer(const std::vector<Vertex>& vertices);
        ~GlVertexBuffer();

        GlVertexBuffer(SelfType&& rhs) noexcept;
        auto operator=(SelfType&& rhs) noexcept -> SelfType&;

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

        void SetupVertexAttributeArray() const;
    private:
        GLuint _GlId{ 0 };
        GlBufferLayout _BufferLayout{};
        GLsizei _VertexCount{ 0 };
    };
}