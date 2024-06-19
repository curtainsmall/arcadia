#pragma once

#include<vector>

#include"core/base.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/mesh/mesh.hpp"

struct GlIndexBuffer: Noncopyable
{
public:
    using self_type = GlIndexBuffer;
public:
    GlIndexBuffer(const std::vector<Mesh::index_type>& indices);
    ~GlIndexBuffer();

    GlIndexBuffer(self_type&& rhs) noexcept;
    auto operator=(self_type&& rhs) noexcept -> self_type&;

    [[nodiscard]]
    auto gl_id() const -> GLuint
    {
        return _gl_id;
    }

    [[nodiscard]]
    auto index_count() const -> GLsizei
    {
        return _index_count;
    }

    void bind() const;
    void unbind() const;
private:
    GLuint _gl_id{ 0 };
    GLsizei _index_count{ 0 };
};
