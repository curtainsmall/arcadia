#pragma once

#include "core/math.hpp"
#include "core/noncopyable.hpp"
#include "platform/defines.hpp"
#include "platform/opengl.hpp"

namespace Arcadia
{
    struct ACDA_API GlRenderbuffer: public Noncopyable
    {
    public:
        using SelfType = GlRenderbuffer;
    public:
        GlRenderbuffer();
        GlRenderbuffer(GLenum format, const glm::i32vec2& size);
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
            const glm::i32vec2& size
        );

    private:
        GLuint _GlId{ 0 };
    };
}