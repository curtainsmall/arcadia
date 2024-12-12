#pragma once

#include<vector>

#include"core/base.hpp"
#include"function/render/opengl/buffer/gl_index_buffer.hpp"
#include"function/render/opengl/buffer/gl_vertex_buffer.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/mesh/mesh.hpp"

class GlVertexArray: public Noncopyable
{
public:
    using SelfType = GlVertexArray;

public:
    GlVertexArray(
        const std::vector<Vertex>& vertices,
        const std::vector<Mesh::index_type>& indices
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

    /// @brief Call @ref DrawArrays, @ref DrawArraysInstanced, @ref DrawIndices or @ref DrawIndicesInstanced based on parameters and state of gl_vertex_array
    /// @param mode OpenGL draw mode
    /// @param count Count of Instance to draw, use @a count < 0 to not draw instanced
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