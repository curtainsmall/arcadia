#pragma once

#include"core/base.hpp"
#include"platform/opengl/opengl_header.hpp"

struct GlUniformBuffer: Noncopyable
{
public:
    using self_type = GlUniformBuffer;
public:
    GlUniformBuffer(
        GLsizeiptr size
    );
    GlUniformBuffer(
        GLsizeiptr size,
        const GLvoid* data
    );
    ~GlUniformBuffer();

    GlUniformBuffer(self_type&& rhs)noexcept;
    auto operator=(self_type&& rhs) noexcept -> self_type&;

    void bind() const;
    void unbind() const;

    void bind_buffer_base(GLuint index) const;
    void bind_buffer_range(GLuint index, GLintptr Offset, GLsizeiptr size) const;

    auto sub_data(GLintptr offset, GLsizeiptr size, const GLvoid* data) const-> const self_type&;

private:
    GLuint _gl_id{ 0 };
};
