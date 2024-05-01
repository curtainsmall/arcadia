#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace Arcadia
{
    struct GlRenderbuffer: Arcadia::Noncopyable
    {
    public:
        using self_type = GlRenderbuffer;
    public:
        GlRenderbuffer();
        GlRenderbuffer(GLenum format, const glm::ivec2& size);
        ~GlRenderbuffer();

        GlRenderbuffer(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        [[nodiscard]]
        auto GetGlId() const ->GLuint
        {
            return _GlId;
        }

        void Bind() const;
        void Unbind() const;

        void SetStorage(
            GLenum format,
            const glm::ivec2& size
        );

    private:
        GLuint _GlId{ 0 };
    };
}
