#pragma once

#include<filesystem>
#include<functional>
#include<unordered_map>
#include<vector>

#include"core/exception.hpp"
#include"core/file/file.hpp"
#include"core/log/log.hpp"
#include"core/math.hpp"
#include"core/noncopyable.hpp"
#include"function/render/opengl/pipeline/gl_shader.hpp"
#include"platform/api_def.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(GlPipelineLinkFailed);
    }

    class GlPipeline: public Noncopyable
    {
    public:

        using GlShadersBuilderType = std::function<void(const std::filesystem::path&, std::vector<GlShader>&)>;

        using SelfType = GlPipeline;
    public:
        GlPipeline(
            const std::filesystem::path& gl_shader_folder_path,
            const GlShadersBuilderType& gl_shaders_builder
        );
        ~GlPipeline();

        GlPipeline(SelfType&& rhs) noexcept;
        auto operator=(SelfType&& rhs) noexcept -> SelfType&;

        [[nodiscard]]
        auto GetGlId() const -> GLuint
        {
            return _GlId;
        }

        void Use() const;
        void Unuse() const;

        auto SetUniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2, GLfloat f3) -> SelfType&;
        auto SetUniform(const std::string& name, GLfloat f0, GLfloat f1, GLfloat f2) -> SelfType&;
        auto SetUniform(const std::string& name, GLfloat f0, GLfloat f1) -> SelfType&;
        auto SetUniform(const std::string& name, GLfloat f) -> SelfType&;
        auto SetUniform(const std::string& name, GLint i0, GLint i1, GLint i2, GLint i3) -> SelfType&;
        auto SetUniform(const std::string& name, GLint i0, GLint i1, GLint i2) -> SelfType&;
        auto SetUniform(const std::string& name, GLint i0, GLint i1) -> SelfType&;
        auto SetUniform(const std::string& name, GLint i) -> SelfType&;
        auto SetUniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2, GLuint u3) -> SelfType&;
        auto SetUniform(const std::string& name, GLuint u0, GLuint u1, GLuint u2) -> SelfType&;
        auto SetUniform(const std::string& name, GLuint u0, GLuint u1) -> SelfType&;
        auto SetUniform(const std::string& name, GLuint u) -> SelfType&;
        auto SetUniform(const std::string& name, const glm::vec4& vec) -> SelfType&;
        auto SetUniform(const std::string& name, const glm::vec3& vec) -> SelfType&;
        auto SetUniform(const std::string& name, const glm::vec2& vec) -> SelfType&;
        auto SetUniform(const std::string& name, const glm::mat4& mat) -> SelfType&;
        auto SetUniform(const std::string& name, const glm::mat3& mat) -> SelfType&;
        auto SetUniform(const std::string& name, const glm::mat2& mat) -> SelfType&;

        auto SetUniformBlockBinding(const std::string& name, GLuint index) -> SelfType&;

    private:
        auto _GetUniformLocation(const std::string& name) -> GLuint;

    private:
        GLuint _GlId{ 0 };
        std::unordered_map<std::string, GLuint> _GlUniformLocationCache{};
        std::vector<GlShader> _GlShaders{};
    };

    static inline auto GenerateModelShadersBuilder() -> GlPipeline::GlShadersBuilderType
    {
        return [](const std::filesystem::path& gl_shader_folder_path, std::vector<GlShader>& gl_shaders) -> void
        {
            auto
                gl_vertex_shader_path = gl_shader_folder_path / "model.vert",
                gl_fragment_shader_path = gl_shader_folder_path / "model.frag";

            auto
                gl_vertex_shader_source = LoadText(gl_vertex_shader_path),
                gl_fragment_shader_source = LoadText(gl_fragment_shader_path);

            gl_shaders.emplace_back(
                gl_vertex_shader_source,
                GlShaderType::VertexShader
            );

            gl_shaders.emplace_back(
                gl_fragment_shader_source,
                GlShaderType::FragementShader
            );
        };
    }

    static inline auto GenerateSkyboxShadersBuilder() -> GlPipeline::GlShadersBuilderType
    {
        return [](const std::filesystem::path& gl_shader_folder_path, std::vector<GlShader>& gl_shaders) -> void
        {
            auto
                gl_vertex_shader_path = gl_shader_folder_path / "skybox.vert",
                gl_fragment_shader_path = gl_shader_folder_path / "skybox.frag";

            auto
                gl_vertex_shader_source = LoadText(gl_vertex_shader_path),
                gl_fragment_shader_source = LoadText(gl_fragment_shader_path);

            gl_shaders.emplace_back(
                gl_vertex_shader_source,
                GlShaderType::VertexShader
            );

            gl_shaders.emplace_back(
                gl_fragment_shader_source,
                GlShaderType::FragementShader
            );
        };
    }

    static inline auto GenerateGridShadersBuilder() -> GlPipeline::GlShadersBuilderType
    {
        return [](const std::filesystem::path& gl_shader_folder_path, std::vector<GlShader>& gl_shaders) -> void
        {
            auto
                gl_vertex_shader_path = gl_shader_folder_path / "grid.vert",
                gl_fragment_shader_path = gl_shader_folder_path / "grid.frag";

            auto
                gl_vertex_shader_source = LoadText(gl_vertex_shader_path),
                gl_fragment_shader_source = LoadText(gl_fragment_shader_path);

            gl_shaders.emplace_back(
                gl_vertex_shader_source,
                GlShaderType::VertexShader
            );

            gl_shaders.emplace_back(
                gl_fragment_shader_source,
                GlShaderType::FragementShader
            );
        };
    }

    static inline auto GenerateShapeShadersBuilder() -> GlPipeline::GlShadersBuilderType
    {
        return [](const std::filesystem::path& gl_shader_folder_path, std::vector<GlShader>& gl_shaders) -> void
        {
            auto
                gl_vertex_shader_path = gl_shader_folder_path / "shape.vert",
                gl_fragment_shader_path = gl_shader_folder_path / "shape.frag";

            auto
                gl_vertex_shader_source = LoadText(gl_vertex_shader_path),
                gl_fragment_shader_source = LoadText(gl_fragment_shader_path);

            gl_shaders.emplace_back(
                gl_vertex_shader_source,
                GlShaderType::VertexShader
            );

            gl_shaders.emplace_back(
                gl_fragment_shader_source,
                GlShaderType::FragementShader
            );
        };
    }
}