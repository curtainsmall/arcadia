#pragma once

#include<filesystem>
#include<functional>
#include<unordered_map>
#include<vector>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/file/file.hpp"
#include"core/log/log.hpp"
#include"core/math.hpp"
#include"function/render/opengl/pipeline/gl_shader.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace arcadia
{
    struct ARCADIA_API gl_pipeline: arcadia::noncopyable
    {
    public:
        ARCADIA_EXCEPTION(link_fail);
        ARCADIA_EXCEPTION(validate_fail);

        using gl_shaders_builder_type = std::function<void(const std::filesystem::path&, std::vector<arcadia::gl_shader>&)>;

        using self_type = gl_pipeline;
    public:
        gl_pipeline(
            const std::filesystem::path& gl_shader_folder_path,
            const gl_shaders_builder_type& gl_shaders_builder
        );
        ~gl_pipeline();

        gl_pipeline(self_type&& rhs) noexcept;
        auto operator=(self_type&& rhs) noexcept -> self_type&;

        [[nodiscard]]
        inline auto get_gl_id() const -> GLuint
        {
            return _gl_id;
        }

        void use() const;
        void unuse() const;

        void set_uniform_4f(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2, GLfloat f3);
        void set_uniform_3f(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2);
        void set_uniform_2f(const std::string& name, GLfloat f0, GLfloat f1);
        void set_uniform_1f(const std::string& name, GLfloat f);
        void set_uniform_1i(const std::string& name, GLint i);
        void set_uniform_vec4(const std::string& name, const glm::vec4& vec);
        void set_uniform_vec3(const std::string& name, const glm::vec3& vec);
        void set_uniform_vec2(const std::string& name, const glm::vec2& vec);
        void set_uniform_mat4(const std::string& name, const glm::mat4& mat);
        void set_uniform_mat3(const std::string& name, const glm::mat3& mat);
        void set_uniform_mat2(const std::string& name, const glm::mat2& mat);

    private:
        auto _get_uniform_location(const std::string& name) -> GLuint;

    private:
        GLuint _gl_id{ 0 };
        std::unordered_map<std::string, GLuint> _gl_uniform_location_cache_umap{};
        std::vector<arcadia::gl_shader> _gl_shaders{};
    };

    static inline auto get_mesh_shaders_builder() -> arcadia::gl_pipeline::gl_shaders_builder_type
    {
        return [](const std::filesystem::path& gl_shader_folder_path, std::vector<arcadia::gl_shader>& gl_shaders) -> void
        {
            auto
                gl_vertex_shader_path = gl_shader_folder_path / "mesh.vert",
                gl_fragment_shader_path = gl_shader_folder_path / "mesh.frag";

            auto
                gl_vertex_shader_source = arcadia::load_text(gl_vertex_shader_path),
                gl_fragment_shader_source = arcadia::load_text(gl_fragment_shader_path);

            gl_shaders.emplace_back(
                gl_vertex_shader_source,
                GL_VERTEX_SHADER
            );

            gl_shaders.emplace_back(
                gl_fragment_shader_source,
                GL_FRAGMENT_SHADER
            );
        };
    }

    static inline auto get_skybox_shaders_builder() -> arcadia::gl_pipeline::gl_shaders_builder_type
    {
        return [](const std::filesystem::path& gl_shader_folder_path, std::vector<arcadia::gl_shader>& gl_shaders) -> void
        {
            auto
                gl_vertex_shader_path = gl_shader_folder_path / "skybox.vert",
                gl_fragment_shader_path = gl_shader_folder_path / "skybox.frag";

            auto
                gl_vertex_shader_source = arcadia::load_text(gl_vertex_shader_path),
                gl_fragment_shader_source = arcadia::load_text(gl_fragment_shader_path);

            gl_shaders.emplace_back(
                gl_vertex_shader_source,
                GL_VERTEX_SHADER
            );

            gl_shaders.emplace_back(
                gl_fragment_shader_source,
                GL_FRAGMENT_SHADER
            );
        };
    }
}
