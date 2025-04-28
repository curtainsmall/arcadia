#pragma once

#include <vector>

#include "core/noncopyable.hpp"
#include "function/render/opengl/buffer/gl_index_buffer.hpp"
#include "function/render/opengl/buffer/gl_vertex_buffer.hpp"
#include "platform/api_def.hpp"
#include "platform/opengl/opengl_header.hpp"
#include "resource/mesh/mesh.hpp"

namespace Arcadia
{
    class GlVertexArray: public Noncopyable
    {
    public:
        using SelfType = GlVertexArray;

    public:
        GlVertexArray(
            const std::vector<Vertex>& vertices,
            const std::vector<Mesh::IndexType>& indices
        );
        ~GlVertexArray();

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