#pragma once

#include <vector>

#include "platform/defines.hpp"
#include "platform/opengl.hpp"
#include "resource/mesh/mesh.hpp"

namespace Arcadia
{
    struct ACDA_API GlIndexBuffer
    {
    public:
        using SelfType = GlIndexBuffer;
    public:
        GlIndexBuffer(const std::vector<Mesh::IndexType>& indices);
        ~GlIndexBuffer();

        GlIndexBuffer(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

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
}