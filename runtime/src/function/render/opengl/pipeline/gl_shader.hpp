#pragma once

#include <string>

#include "core/noncopyable.hpp"
#include "platform/api_def.hpp"
#include "platform/opengl_header.hpp"

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

    namespace Exceptions
    {
        ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION(GlShaderCompileFail);
    }

    class GlShader: public Noncopyable
    {
    public:

        using SelfType = GlShader;
    public:
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