#pragma once

#include <vector>

#include "function/render/opengl/buffer/gl_index_buffer.hpp"
#include "function/render/opengl/buffer/gl_vertex_buffer.hpp"
#include "platform/defines.hpp"
#include "platform/opengl.hpp"
#include "resource/mesh/mesh.hpp"

namespace Arcadia
{
    struct ACDA_API GlVertexArray
    {
    public:
        using SelfType = GlVertexArray;

    public:
        GlVertexArray(
            const std::vector<Vertex>& vertices,
            const std::vector<Mesh::IndexType>& indices
        );
        ~GlVertexArray();

        GlVertexArray(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        GlVertexArray(SelfType&& rhs) noexcept;
        auto operator=(SelfType&& rhs) noexcept -> SelfType&;

        [[nodiscard]]
        auto GetGlId() const -> GLuint
        {
            return _GlId;
        }

        void Bind() const;
        void Unbind() const;

        void Draw(GLenum mode, GLsizei count = -1) const;
        void DrawArrays(GLenum mode) const;
        void DrawArraysInstanced(GLenum mode, GLsizei count) const;
        void DrawIndices(GLenum mode) const;
        void DrawIndicesInstanced(GLenum mode, GLsizei count) const;

    private:
        GLuint _GlId{ 0 };

        GlVertexBuffer _GlVertexBuffer;
        GlIndexBuffer _GlIndexBuffer;
    };
}