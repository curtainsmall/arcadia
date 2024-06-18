#include "pch.hpp"

#include "gl_renderer.hpp"

#include<vector>

#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/skybox_component.hpp"
#include"resource/components/transform_component.hpp"

GlRenderer::GlRenderer(const std::filesystem::path& gl_shader_folder_path):
    _GlModelPipeline(gl_shader_folder_path, GetModelShadersBuilder()),
    _GlSkyboxPipeline(gl_shader_folder_path, GetSkyboxShadersBuilder()),
    _GlGridPipeline(gl_shader_folder_path, GetGridShadersBuilder()),
    _GlShapePipeline(gl_shader_folder_path, GetShapeShadersBuilder())
{
    ACDA_GL_CALL(glEnable(GL_DEPTH_TEST));
    ACDA_GL_CALL(glEnable(GL_CULL_FACE));
}

void GlRenderer::Prepare()
{
    _AssertFrameNotInBuild();
    _InBuild = true;

    // Clear submitted meshes uuids
    _SubmittedMeshesUuids.clear();

    // Clear cameras
    _GlRenderUnitCameras.clear();

    // Clear lights
    _GlRenderUnitLights.clear();

    // Clear skybox
    _optGlRenderUnitSkybox.reset();

    // Clear submitted physics body shape uuids
    _SubmittedPhysicsBodyShapeUuids.clear();

}

void GlRenderer::Finalize()
{
    _AssertFrameInBuild();
    _InBuild = false;

    for(auto iter = _GlRenderUnitMeshStorage.begin(); iter != _GlRenderUnitMeshStorage.end();)
    {
        if(!_SubmittedMeshesUuids.contains(iter->first))
        {
            iter = _GlRenderUnitMeshStorage.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    for(auto iter = _GlRenderUnitPhysicsBodyShapeStorage.begin(); iter != _GlRenderUnitPhysicsBodyShapeStorage.end();)
    {
        if(!_SubmittedPhysicsBodyShapeUuids.contains(iter->first))
        {
            iter = _GlRenderUnitPhysicsBodyShapeStorage.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void GlRenderer::Submit(const Scene& scene, const std::string& name)
{
    _AssertFrameInBuild();

    const auto& entity_info = scene.GetEntityInfo(name);

    Match<void>(
        entity_info.Type,
        [&]()
    {
        ACDA_ASSERT(false && "Entity type not supported");
    },
        "camera"s,
        [&]()
    {
        const auto& [camera_comp, transform_comp] = scene.Get<CameraComponent, TransformComponent>(name);

        _GlRenderUnitCameras.emplace_back(
            GlFramebuffer{
                camera_comp.ViewportSize,
                camera_comp.NearPlane,
                camera_comp.FarPlane
            },
            camera_comp.ViewportSize,
            camera_comp.GenerateViewMat4(transform_comp.Position, transform_comp.Direction),
            camera_comp.GenerateProjMat4(),
            transform_comp.Position,
            camera_comp.ShouldDisplayGrid,
            camera_comp.NearPlane,
            camera_comp.FarPlane
        );
    },
        "light"s,
        [&]()
    {
        const auto& [light_comp, transform_comp] = scene.Get<LightComponent, TransformComponent>(name);
        _GlRenderUnitLights.emplace_back(transform_comp.Position, transform_comp.Direction, light_comp.Light);
    },
        "actor"s,
        [&]()
    {
        const auto [model_comp, transform_comp, physics_comp] = scene.Get<ModelComponent, TransformComponent, PhysicsComponent>(name);

        if(model_comp.HasIdentifiableMeshes())
        {
            const auto& [uuid, meshes] = model_comp.GetIdentifiableMeshes();

            const auto transform_mat = transform_comp.GenerateTransformMatrix();

            if(!_GlRenderUnitMeshStorage.contains(uuid))
            {

                std::vector<GlRenderUnitMesh> gl_meshes{};
                for(const auto& mesh : meshes)
                {
                    // For any uuid, its corresponding meshes must be the same
                    gl_meshes.emplace_back(
                        GlVertexArray{ mesh.Vertices, mesh.Indices },
                        transform_mat,
                        mesh.Material.AmbientTexture2d,
                        mesh.Material.DiffuseTexture2d,
                        mesh.Material.SpecularTexture2d
                    );
                }
                _GlRenderUnitMeshStorage.try_emplace(uuid, std::move(gl_meshes));
            }
            else
            {
                for(auto& gl_render_unit_mesh : _GlRenderUnitMeshStorage.at(uuid))
                {
                    std::get<1>(gl_render_unit_mesh) = transform_mat;
                }
            }

            _SubmittedMeshesUuids.emplace(uuid);
        }

        if(physics_comp.HasBodyInfo())
        {
            const auto& [uuid, jph_body] = physics_comp.GetIdentifiableJphBodyInfo();
            if(!_GlRenderUnitPhysicsBodyShapeStorage.contains(uuid))
            {
                const auto& shape_info = jph_body.JphShapeInfo;
                const auto& shape_mesh = Match<Mesh>(
                    shape_info,
                    [&](const JphBoxShapeInfo& info)
                {
                    return Mesh::Box(info.HalfExtent);
                },
                    [&](const JphCapsuleShapeInfo& info)
                {
                    return Mesh::Capsule(info.Radius, info.HalfHeightOfCylinder);
                },
                    [&](const JphCylinderShapeInfo& info)
                {
                    return Mesh::Cylinder(info.HalfHeight, info.Radius);
                },
                    [&](const JphSphereShapeInfo& info)
                {
                    return Mesh::Sphere(info.Radius);
                }
                );

                _GlRenderUnitPhysicsBodyShapeStorage.try_emplace(
                    uuid, GlVertexArray{ shape_mesh.Vertices,shape_mesh.Indices },
                    glm::mat4{},
                    glm::vec3{}
                );
            }

            auto& [GlVertexBuffer, transform_mat, color] = _GlRenderUnitPhysicsBodyShapeStorage.at(uuid);
            transform_mat = glm::translate(
                glm::mat4_cast(transform_comp.Rotation),
                transform_comp.Position
            );
            color = physics_comp.BodyShapeColor;

            _SubmittedPhysicsBodyShapeUuids.emplace(uuid);
        }
    }
    );
}

void GlRenderer::Draw()
{
    _AssertFrameNotInBuild();


    if(_GlRenderUnitCameras.empty())
    {
        throw DrawFail{ "No framebuffer to draw to" };
    }


    ACDA_GL_CALL(glClearColor(41 / 255.0, 43 / 255.0, 44 / 255.0, 1.f));

    // For each framebuffer
    for(const auto& [
        gl_framebuffer,
            viewport_size,
            camera_view,
            camera_proj,
            camera_position,
            should_display_grid,
            near_plane,
            far_plane
    ] : _GlRenderUnitCameras)
    {
        gl_framebuffer.Bind();

        // Clear framebufers
        ACDA_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        ACDA_GL_CALL(glViewport(0, 0, viewport_size.x, viewport_size.y));

        // Draw grid
        if(should_display_grid)
        {
            std::vector<Vertex> grid_vertices{
                   Vertex{ glm::vec3{-1,1,0} },
                    Vertex{ glm::vec3{-1,-1,0} },
                    Vertex{ glm::vec3{1,-1,0} },
                    Vertex{ glm::vec3{1,1,0} }
            };
            std::vector<Mesh::index_type> grid_indices{
                0,1,2,
                2,3,0
            };
            GlVertexArray gl_grid_vertex_array{ grid_vertices, grid_indices };

            _DrawGrid(
                gl_grid_vertex_array,
                camera_view,
                camera_proj,
                near_plane,
                far_plane
            );
        }

        // Lights
        const GLsizeiptr light_t_size{ 128 };
        const int max_light_count = 32;
        const int light_count_size_aligned = 16; // Sizeof `u_light_count` in fragment shader with alignment considered
        GlUniformBuffer GlUniformBuffer{ light_count_size_aligned + light_t_size * max_light_count };

        auto light_box_shape = Mesh::Box(glm::vec3{ 1,1,1 });
        GlVertexArray gl_light_box_shape_vertex_array{ light_box_shape.Vertices ,light_box_shape.Indices };

        _DrawLights(
            light_t_size,
            max_light_count,
            light_count_size_aligned,
            GlUniformBuffer,
            gl_light_box_shape_vertex_array,
            camera_view,
            camera_proj
        );

        // Draw with mesh pipeline
        _DrawModels(
            camera_view,
            camera_proj,
            camera_position
        );

        _DrawPhysicsBodyShape(
            camera_view,
            camera_proj
        );

        // Draw with skybox pipeline
        if(_optGlRenderUnitSkybox)
        {
            _DrawSkybox(
                camera_view,
                camera_proj
            );
        }

        gl_framebuffer.Unbind();
    }

}

void GlRenderer::Reset()
{
    _GlRenderUnitCameras.clear();
    _GlRenderUnitLights.clear();
    _GlRenderUnitMeshStorage.clear();
    _SubmittedMeshesUuids.clear();
    _optGlRenderUnitSkybox.reset();
}

auto GlRenderer::GetRenderResultId(std::size_t index) const -> void*
{
    return reinterpret_cast<void*>(std::get<0>(_GlRenderUnitCameras.at(index)).GetGlTexture2d().GetGlId());
}

void GlRenderer::_AssertFrameInBuild() const
{
    ACDA_ASSERT(_InBuild && "Frame is not in build, did you call `prepare()`?");
}

void GlRenderer::_AssertFrameNotInBuild() const
{
    ACDA_ASSERT(!_InBuild && "Frame is in build, did you call `finalize()`?");
}

void GlRenderer::_DrawGrid(
    const GlVertexArray& gl_grid_vertex_array,
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj,
    float near_plane,
    float far_plane
)
{
    _GlGridPipeline.Use();
    _GlGridPipeline
        .SetUniform("u_view_mat4", camera_view)
        .SetUniform("u_proj_mat4", camera_proj)
        .SetUniform("u_near_plane", near_plane)
        .SetUniform("u_far_plane", far_plane);

    gl_grid_vertex_array.Bind();
    gl_grid_vertex_array.Draw(GL_TRIANGLES);
    gl_grid_vertex_array.Unbind();

    _GlGridPipeline.Unuse();
}

void GlRenderer::_DrawLights(
    const GLsizeiptr light_t_size,
    const int max_light_count,
    const int light_count_size_aligned,
    GlUniformBuffer& gl_light_uniform_buffer,
    const GlVertexArray& gl_light_shape_vertex_array,
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj
)
{
    const int light_type_none = 0;
    const int light_type_spot = 1;
    const int light_type_direct = 2;
    const int light_type_area = 3;
    const int light_type_point = 4;

    _GlShapePipeline.Use();
    _GlShapePipeline
        .SetUniform("u_view_mat", camera_view)
        .SetUniform("u_proj_mat", camera_proj);
    gl_light_shape_vertex_array.Bind();

    GLsizeiptr light_count = 0;
    for(const auto& [position, direction, light] : _GlRenderUnitLights)
    {
        if(light_count > max_light_count)
        {
            throw too_many_lights{ std::format("The max light count is {}",max_light_count) };
        }

        Match<void>(
            light,
            [&](const NullLight& light)
        {},
            [&](const SpotLight& light)
        {
            GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
            gl_light_uniform_buffer.SubData(base_offfset + 0, sizeof(int), &light_type_spot);
            float cosine_inner_cutoff_angle = glm::cos(light.CutoffAngles.x);
            float cosine_outer_cutoff_angle = glm::cos(light.CutoffAngles.y);
            gl_light_uniform_buffer
                .SubData(base_offfset + 4, sizeof(float), &cosine_inner_cutoff_angle)
                .SubData(base_offfset + 8, sizeof(float), &cosine_outer_cutoff_angle)
                .SubData(base_offfset + 16, sizeof(glm::vec3), &position)
                .SubData(base_offfset + 32, sizeof(glm::vec3), &direction)
                .SubData(base_offfset + 48, sizeof(glm::vec3), &light.AttenuationCoefs)
                .SubData(base_offfset + 64, sizeof(glm::vec3), &light.Color)
                .SubData(base_offfset + 80, sizeof(glm::vec3), &light.AmbientStrength)
                .SubData(base_offfset + 96, sizeof(glm::vec3), &light.DiffuseStrength)
                .SubData(base_offfset + 112, sizeof(glm::vec3), &light.SpecularStrength);
            ++light_count;

            _GlShapePipeline
                .SetUniform("u_transform_mat", glm::translate(Mat4::Identity(), position))
                .SetUniform("u_color", light.Color);
        },
            [&](const DirectLight& light)
        {
            GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
            gl_light_uniform_buffer
                .SubData(base_offfset + 0, sizeof(int), &light_type_direct)
                .SubData(base_offfset + 32, sizeof(glm::vec3), &direction)
                .SubData(base_offfset + 64, sizeof(glm::vec3), &light.Color)
                .SubData(base_offfset + 80, sizeof(glm::vec3), &light.AmbientStrength)
                .SubData(base_offfset + 96, sizeof(glm::vec3), &light.DiffuseStrength)
                .SubData(base_offfset + 112, sizeof(glm::vec3), &light.SpecularStrength);
            ++light_count;
        },
            [&](const AreaLight& light)
        {},
            [&](const PointLight& light)
        {
            GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
            gl_light_uniform_buffer
                .SubData(base_offfset + 0, sizeof(int), &light_type_point)
                .SubData(base_offfset + 16, sizeof(glm::vec3), &position)
                .SubData(base_offfset + 48, sizeof(glm::vec3), &light.AttenuationCoefs)
                .SubData(base_offfset + 64, sizeof(glm::vec3), &light.Color)
                .SubData(base_offfset + 80, sizeof(glm::vec3), &light.AmbientStrength)
                .SubData(base_offfset + 96, sizeof(glm::vec3), &light.DiffuseStrength)
                .SubData(base_offfset + 112, sizeof(glm::vec3), &light.SpecularStrength);
            ++light_count;

            _GlShapePipeline
                .SetUniform("u_transform_mat", glm::translate(Mat4::Identity(), position))
                .SetUniform("u_color", light.Color);
        }
        );

        gl_light_shape_vertex_array.Draw(GL_TRIANGLES);

    }
    gl_light_shape_vertex_array.Unbind();
    _GlShapePipeline.Unuse();
    gl_light_uniform_buffer.SubData(0, sizeof(int), &light_count);
    gl_light_uniform_buffer.BindBufferBase(0);
}

void GlRenderer::_DrawModels(
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj,
    const glm::vec3& camera_pos
)
{
    _GlModelPipeline.Use();
    auto tex_uniform_index = 0;
    _GlModelPipeline
        .SetUniform("u_view_mat", camera_view)
        .SetUniform("u_proj_mat", camera_proj)
        .SetUniform("u_view_pos", camera_pos);
    for(auto& [Uuid, gl_meshes] : _GlRenderUnitMeshStorage)
    {
        for(auto& [
            gl_vertex_array,
                transform_mat4,
                gl_texture2d_ambient,
                gl_texture2d_diffuse,
                gl_texture2d_specular
        ] : gl_meshes)
        {
            _GlModelPipeline
                .SetUniform("u_transform_mat", transform_mat4)
                .SetUniform("u_normal_mat", glm::mat3{ glm::transpose(glm::inverse(transform_mat4)) });

            _GlModelPipeline.SetUniform("u_material.ambient", tex_uniform_index);
            gl_texture2d_ambient.Bind(tex_uniform_index++);

            _GlModelPipeline.SetUniform("u_material.diffuse", tex_uniform_index);
            gl_texture2d_diffuse.Bind(tex_uniform_index++);

            _GlModelPipeline.SetUniform("u_material.specular", tex_uniform_index);
            gl_texture2d_specular.Bind(tex_uniform_index++);

            _GlModelPipeline.SetUniform("u_material.shininess", 32.f);

            gl_vertex_array.Bind();
            gl_vertex_array.Draw(GL_TRIANGLES);
            gl_vertex_array.Unbind();

            gl_texture2d_ambient.Unbind();
            gl_texture2d_diffuse.Unbind();
            gl_texture2d_specular.Unbind();

        }
    }
    _GlModelPipeline.Unuse();
}

void GlRenderer::_DrawSkybox(
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj
)
{
    _GlSkyboxPipeline.Use();
    auto& [gl_vartex_array, gl_cubemap] = *_optGlRenderUnitSkybox;

    _GlSkyboxPipeline.SetUniform("u_skybox", 0);
    gl_cubemap.Bind(0);

    _GlSkyboxPipeline
        .SetUniform("u_view_mat", glm::mat4{ glm::mat3{camera_view} })
        .SetUniform("u_proj_mat", camera_proj);

    gl_vartex_array.Bind();
    ACDA_GL_CALL(glDepthFunc(GL_LEQUAL));
    gl_vartex_array.Draw(GL_TRIANGLES);
    ACDA_GL_CALL(glDepthFunc(GL_LESS));
    gl_vartex_array.Unbind();

    _GlSkyboxPipeline.Unuse();
}

void GlRenderer::_DrawPhysicsBodyShape(
    const glm::mat4& camera_view,
    const glm::mat4& camera_proj
)
{
    _GlShapePipeline.Use();
    for(const auto& [gl_vertex_arrray, transform_mat, color] : _GlRenderUnitPhysicsBodyShapeStorage | std::views::values)
    {
        _GlShapePipeline
            .SetUniform("u_transform_mat", transform_mat)
            .SetUniform("u_view_mat", camera_view)
            .SetUniform("u_proj_mat", camera_proj)
            .SetUniform("u_color", color);

        gl_vertex_arrray.Bind();
        gl_vertex_arrray.Draw(GL_TRIANGLES);
        gl_vertex_arrray.Unbind();
    }
    _GlShapePipeline.Unuse();
}




