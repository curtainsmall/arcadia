#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"platform/opengl/opengl_header.hpp"

struct GlRenderbuffer: Noncopyable
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
    auto gl_id() const ->GLuint
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
