#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace Arcadia
{
    class GlRenderbuffer: public Noncopyable
    {
    public:
        using SelfType = GlRenderbuffer;
    public:
        GlRenderbuffer();
        GlRenderbuffer(GLenum format, const glm::ivec2& size);
        ~GlRenderbuffer();

        GlRenderbuffer(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

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