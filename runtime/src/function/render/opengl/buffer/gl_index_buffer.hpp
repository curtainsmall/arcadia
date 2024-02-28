#pragma once

#include<vector>

#include"core/base.hpp"
#include"platform/opengl/opengl_header.hpp"
#include"resource/component/model_component/mesh/mesh.hpp"

namespace arcadia
{
    struct ARCADIA_API gl_index_buffer: arcadia::noncopyable
    {
    public:
        using self_type = gl_index_buffer;
    public:
        gl_index_buffer(const std::vector<arcadia::mesh::index_type>& indices);
        ~gl_index_buffer();

        gl_index_buffer(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        inline auto get_id() const -> GLuint
        {
            return _gl_index_buffer_id;
        }

        [[nodiscard]]
        inline auto get_index_count() const -> GLsizei
        {
            return _index_count;
        }

        void bind() const;
        void unbind() const;
    private:
        GLuint _gl_index_buffer_id{ 0 };
        GLsizei _index_count{ 0 };
    };
}
