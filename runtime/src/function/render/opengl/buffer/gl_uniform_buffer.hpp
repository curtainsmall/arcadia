#pragma once

#include"core/base.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace arcadia
{
    struct ARCADIA_API gl_uniform_buffer: arcadia::noncopyable
    {
    public:
        using self_type = gl_uniform_buffer;
    public:
        gl_uniform_buffer(
            GLsizeiptr size
        );
        gl_uniform_buffer(
            GLsizeiptr size,
            const GLvoid* data
        );
        ~gl_uniform_buffer();

        gl_uniform_buffer(self_type&& rhs)noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        void bind() const;
        void unbind() const;

        void bind_buffer_base(GLuint index) const;
        void bind_buffer_range(GLuint index, GLintptr offset, GLsizeiptr size) const;

        void sub_data(GLintptr offset, GLsizeiptr size, const GLvoid* data) const;

    private:
        GLuint _gl_id{ 0 };
    };
}
