#pragma once

#include<vector>

#include"core/base.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/mesh/mesh.hpp"

class GlIndexBuffer: public Noncopyable
{
public:
    using SelfType = GlIndexBuffer;
public:
    GlIndexBuffer(const std::vector<Mesh::index_type>& indices);
    ~GlIndexBuffer();

    GlIndexBuffer(SelfType&& rhs) noexcept;
    auto operator=(SelfType&& rhs) noexcept -> SelfType&;

    [[nodiscard]]
    auto GetGlId() const -> GLuint
    {
        return _GlId;
    }

    [[nodiscard]]
    auto GetIndexCount() const -> GLsizei
    {
        return _IndexCount;
    }

    void Bind() const;
    void Unbind() const;
private:
    GLuint _GlId{ 0 };
    GLsizei _IndexCount{ 0 };
};