#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core/exception.hpp"
#include "core/math.hpp"
#include "function/render/opengl/buffer/gl_cubemap.hpp"
#include "function/render/opengl/buffer/gl_framebuffer.hpp"
#include "function/render/opengl/buffer/gl_uniform_buffer.hpp"
#include "function/render/opengl/buffer/gl_vertex_array.hpp"
#include "function/render/opengl/pipeline/gl_pipeline.hpp"
#include "function/render/renderer.hpp"
#include "platform/api_def.hpp"
#include "platform/opengl.hpp"
#include "resource/components/light_component.hpp"

namespace Arcadia
{
    class GlRenderUnitCamera
    {
    public:
        GlFramebuffer Framebuffer;
        glm::i32vec2 ViewportSize;
        glm::mat4 CameraViewMatrix;
        glm::mat4 CameraProjectionMatrix;
        glm::vec3 CameraPosition;
        bool GridDisplaying;
        float NearPlane;
        float FarPlane;
    };

    class GlRenderUnitLight
    {
    public:
        glm::vec3 Position;
        glm::vec3 Direction;
        LightType Light;
    };

    class GlRenderUnitMesh
    {
    public:
        GlVertexArray VertexArray;
        glm::mat4 TransformMatrix;
        GlTexture2d AmbientTexture;
        GlTexture2d DiffuseTexture;
        GlTexture2d SpecularTexture;
    };

    class GlRenderUnitSkybox
    {
    public:
        GlVertexArray VertexArray;
        GlCubemap Cubemap;
    };

    class GlRenderUnitPhysicsBodyShape
    {
    public:
        GlVertexArray VertexArray;
        glm::mat4 TransformMatrix;
        glm::vec3 Color;
    };

    class GlRenderer: public RendererInterface
    {
    public:

        using SelfType = GlRenderer;
    private:
        enum class _BuildHint: std::uint8_t
        {
            BuildAll = 0,
            UpdateAll,
            UpdateJustTransformMatrix,
        };
    public:
        GlRenderer(const std::filesystem::path& gl_shader_folder_path);
        virtual ~GlRenderer() override = default;

        [[nodiscard]]
        virtual auto HasEntity(EntityId entity_id) const -> bool override;
        virtual void AddEntity(EntityId entity_id) override;
        virtual void RemoveEntity(EntityId entity_id) override;
        virtual void UpdateEntity(EntityId entity_id) override;
        virtual void Draw() override;
        virtual void Reset() override;

        [[nodiscard]]
        virtual auto HasRenderResult() const -> bool;
        [[nodiscard]]
        virtual auto GetRenderResultId(EntityId entity_id) const->void* override;
        [[nodiscard]]
        virtual auto GetGraphicApiType() const->GraphicApi::Type override;

    public:
        void _BuildForEntity(EntityId entity_id, _BuildHint hint);
        void _ClearForEntity(EntityId entity_id);

        void _DrawGrid(
            const GlVertexArray& gl_grid_vertex_array,
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj,
            float near_plane,
            float far_plane
        );
        void _DrawLights(
            const GLsizeiptr light_t_size,
            const std::int32_t max_light_count,
            const std::int32_t light_count_size_aligned,
            GlUniformBuffer& gl_light_uniform_buffer,
            const GlVertexArray& gl_light_shape_vertex_array,
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj
        );
        void _DrawModels(
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj,
            const glm::vec3& camera_pos
        );
        void _DrawSkybox(
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj
        );
        void _DrawPhysicsBodyShape(
            const glm::mat4& camera_view,
            const glm::mat4& camera_proj
        );

    private:
        std::unordered_map<EntityId, std::vector<GlRenderUnitMesh>> _GlRenderUnitMeshStorage{};
        std::unordered_map<EntityId, GlRenderUnitPhysicsBodyShape> _GlRenderUnitPhysicsBodyShapeStorage{};
        std::unordered_map<EntityId, GlRenderUnitCamera> _GlRenderUnitCameraStorage{};
        std::unordered_map<EntityId, GlRenderUnitLight> _GlRenderUnitLightStorage{};
        std::optional<GlRenderUnitSkybox> _GlRenderUnitSkybox{};
        std::unordered_set<EntityId> _EntityIdSet{};

        GlPipeline _GlModelPipeline;
        GlPipeline _GlSkyboxPipeline;
        GlPipeline _GlGridPipeline;
        GlPipeline _GlShapePipeline;
    };
}