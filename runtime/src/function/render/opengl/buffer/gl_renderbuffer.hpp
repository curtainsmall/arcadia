#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace arcadia
{
    struct ARCADIA_API gl_renderbuffer: arcadia::noncopyable
    {
    public:
        using self_type = gl_renderbuffer;
    public:
        gl_renderbuffer();
        gl_renderbuffer(GLenum format, const glm::ivec2& size);
        ~gl_renderbuffer();

        gl_renderbuffer(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        [[nodiscard]]
        auto get_gl_id() const ->GLuint
        {
            return _gl_id;
        }

        void bind() const;
        void unbind() const;

        void set_storage(
            GLenum format,
            const glm::ivec2& size
        );

    private:
        GLuint _gl_id{ 0 };
    };
}
