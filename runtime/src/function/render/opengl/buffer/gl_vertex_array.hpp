#pragma once

#include<vector>

#include"core/base.hpp"
#include"function/render/opengl/buffer/gl_index_buffer.hpp"
#include"function/render/opengl/buffer/gl_vertex_buffer.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/mesh/mesh.hpp"

namespace Arcadia
{

    struct GlVertexArray: Arcadia::Noncopyable
    {
    public:
        using self_type = GlVertexArray;

    public:
        GlVertexArray(
            const std::vector<Arcadia::Vertex>& vertices,
            const std::vector<Arcadia::Mesh::index_type>& indices
        );
        ~GlVertexArray();

        GlVertexArray(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        auto GetGlId() const -> GLuint
        {
            return _GlId;
        }

        void Bind() const;
        void Unbind() const;

        /// @brief Call @ref DrawArrays, @ref DrawArraysInstanced, @ref DrawIndices or @ref DrawIndicesInstanced based on parameters and state of gl_vertex_array
        /// @param mode OpenGL draw mode
        /// @param count Count of instance to draw, use count < 0 to not draw instanced
        void Draw(GLenum mode, GLsizei count = -1) const;

        void DrawArrays(GLenum mode) const;
        void DrawArraysInstanced(GLenum mode, GLsizei count) const;
        void DrawIndices(GLenum mode) const;
        void DrawIndicesInstanced(GLenum mode, GLsizei count) const;

    private:
        GLuint _GlId{ 0 };

        Arcadia::GlVertexBuffer _GlVertexBuffer;
        Arcadia::GlIndexBuffer _GlIndexBuffer;
    };
}
