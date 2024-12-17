#pragma once

#include<string>

#include"core/noncopyable.hpp"
#include"platform/api_def.hpp"
#include"platform/opengl/opengl_header.hpp"

namespace Arcadia
{
    enum class GlShaderType: unsigned int
    {
        None = GL_NONE,
        VertexShader = GL_VERTEX_SHADER,
        FragementShader = GL_FRAGMENT_SHADER,
        ComputeShader = GL_COMPUTE_SHADER,
        GeometryShader = GL_GEOMETRY_SHADER,
        TessControlShader = GL_TESS_CONTROL_SHADER,
        TessEvaluationShader = GL_TESS_EVALUATION_SHADER,
    };

    class GlShader: public Noncopyable
    {
    public:
        ACDA_DEFINE_EXCEPTION(CompileFail);

        using SelfType = GlShader;
    public:
        /// @brief Create an OpenGL shader object
        /// @param source_code Source code of the shader
        /// @param shader_type Type of shader
        GlShader(
            const std::string& source_code,
            GlShaderType shader_type
        );
        ~GlShader();

        GlShader(SelfType&& rhs) noexcept;
        auto operator=(SelfType&& rhs) noexcept -> SelfType&;

        [[nodiscard]]
        auto GetGlId() const -> GLuint
        {
            return _GlId;
        }
    private:
        GLuint _GlId{ 0 };
    };
}