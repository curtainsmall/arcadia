#pragma once

#include<vector>

#include"core/base.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/mesh/mesh.hpp"

namespace Arcadia
{
    struct GlIndexBuffer: Arcadia::Noncopyable
    {
    public:
        using self_type = GlIndexBuffer;
    public:
        GlIndexBuffer(const std::vector<Arcadia::Mesh::index_type>& indices);
        ~GlIndexBuffer();

        GlIndexBuffer(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

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
}
