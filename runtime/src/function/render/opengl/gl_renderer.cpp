#include "pch.hpp"
#include "gl_renderer.hpp"

#include<vector>

arcadia::gl_renderer::gl_renderer(const std::filesystem::path& gl_shader_folder_path):
    _gl_mesh_pipeline(gl_shader_folder_path, arcadia::get_model_shaders_builder()),
    _gl_skybox_pipeline(gl_shader_folder_path, arcadia::get_skybox_shaders_builder()),
    _gl_grid_pipeline(gl_shader_folder_path, arcadia::get_grid_shaders_builder()),
    _gl_icon_pipeline(gl_shader_folder_path, arcadia::get_icon_shaders_builder())
{
    ARCADIA_GL_CALL(glEnable(GL_DEPTH_TEST));
    ARCADIA_GL_CALL(glEnable(GL_CULL_FACE));
}

void arcadia::gl_renderer::prepare()
{
    _assert_frame_not_in_build();
    _in_build = true;

    // Clear submitted meshes uuids
    _submitted_meshes_uuid_set.clear();

    // Clear cameras
    _gl_render_unit_cameras.clear();

    // Clear lights
    _gl_render_unit_lights.clear();

    // Clear skybox
    _gl_render_unit_skybox_opt.reset();

}

void arcadia::gl_renderer::finalize()
{
    _assert_frame_in_build();
    _in_build = false;

    // Remove gl_render_unit_mesh-es that have been submitted in previous frames but not in this frame
    for(auto iter = _gl_render_unit_meshes_umap.begin(); iter != _gl_render_unit_meshes_umap.end();)
    {
        if(!_submitted_meshes_uuid_set.contains(iter->first))
        {
            iter = _gl_render_unit_meshes_umap.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void arcadia::gl_renderer::submit(const arcadia::camera_component& camera_comp)
{
    _assert_frame_in_build();

    _gl_render_unit_cameras.emplace_back(
        arcadia::gl_framebuffer{
            camera_comp.viewport_size,
            camera_comp.near_plane,
            camera_comp.far_plane
        },
        camera_comp.viewport_size,
        camera_comp.build_view_mat4(),
        camera_comp.build_proj_mat4(),
        camera_comp.pos,
        camera_comp.should_display_grid,
        camera_comp.near_plane,
        camera_comp.far_plane
    );
}

void arcadia::gl_renderer::submit(const arcadia::light_component& light_comp)
{
    _assert_frame_in_build();

    _gl_render_unit_lights.emplace_back(light_comp.light);
}

void arcadia::gl_renderer::submit(const arcadia::model_component& model_comp)
{
    _assert_frame_in_build();

    if(model_comp.has_identifiable_meshes())
    {
        const auto& [uuid, meshes] = model_comp.get_identifiable_meshes();
        if(!_gl_render_unit_meshes_umap.contains(uuid))
        {
            auto transform_mat =
                // Translate
                glm::translate(
                    // Move pivot back from origin
                    glm::translate(
                        // Rotate about z-axis
                        glm::rotate(
                            // Rotate about y-axis
                            glm::rotate(
                                // Rotate about x-axis
                                glm::rotate(
                                    // Scale about origin (same as pivot)
                                    glm::scale(
                                        // Move pivot to origin
                                        glm::translate(
                                            arcadia::mat4::identity(),
                                            -model_comp.pivot
                                        ),
                                        model_comp.scale
                                    ),
                                    model_comp.rotation.x,
                                    arcadia::vec3::pos_unit_x()
                                ),
                                model_comp.rotation.y,
                                arcadia::vec3::pos_unit_y()
                            ),
                            model_comp.rotation.z,
                            arcadia::vec3::pos_unit_z()
                        ),
                        model_comp.pivot
                    ),
                    model_comp.location
                );

            std::vector<arcadia::gl_render_unit_mesh> gl_meshes{};
            for(const auto& mesh : meshes)
            {
                // For any uuid, its corresponding meshes must be the same
                gl_meshes.emplace_back(
                    arcadia::gl_vertex_array{ mesh.vertices, mesh.indices },
                    transform_mat,
                    mesh.material.ambient_texture2d,
                    mesh.material.diffuse_texture2d,
                    mesh.material.specular_texture2d
                );
            }
            _gl_render_unit_meshes_umap.try_emplace(uuid, std::move(gl_meshes));
        }
        _submitted_meshes_uuid_set.emplace(uuid);
    }
}

void arcadia::gl_renderer::submit(const arcadia::skybox_component& skybox_comp)
{
    _assert_frame_in_build();

    const auto& [vertices, indices] = _create_unit_cube_mesh();
    _gl_render_unit_skybox_opt.emplace(
        arcadia::gl_vertex_array{ vertices,indices },
        skybox_comp.cubemap
    );
}

void arcadia::gl_renderer::draw()
{
    _assert_frame_not_in_build();


    if(_gl_render_unit_cameras.empty())
    {
        throw draw_fail{ "No framebuffer to draw to" };
    }


    ARCADIA_GL_CALL(glClearColor(41 / 255.0, 43 / 255.0, 44 / 255.0, 1.f));

    // For each framebuffer
    for(const auto& [
        gl_framebuffer,
            viewport_size,
            camera_view_mat4,
            camera_proj_mat4,
            camera_position,
            should_display_grid,
            near_plane,
            far_plane
    ] : _gl_render_unit_cameras)
    {
        gl_framebuffer.bind();

        // Clear framebufers
        ARCADIA_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        // Draw grid
        if(should_display_grid)
        {
            std::vector<arcadia::vertex> grid_vertices{
               arcadia::vertex{ glm::vec3{-1,1,0} },
                arcadia::vertex{ glm::vec3{-1,-1,0} },
                arcadia::vertex{ glm::vec3{1,-1,0} },
                arcadia::vertex{ glm::vec3{1,1,0} }
            };
            std::vector<arcadia::mesh::index_type> grid_indices{
                0,1,2,
                2,3,0
            };
            arcadia::gl_vertex_array gl_grid_vertex_array{ grid_vertices, grid_indices };

            _gl_grid_pipeline.use();
            _gl_grid_pipeline.set_uniform("u_view_mat4", camera_view_mat4);
            _gl_grid_pipeline.set_uniform("u_proj_mat4", camera_proj_mat4);
            _gl_grid_pipeline.set_uniform("u_near_plane", near_plane);
            _gl_grid_pipeline.set_uniform("u_far_plane", far_plane);

            gl_grid_vertex_array.bind();
            gl_grid_vertex_array.draw_indices(GL_TRIANGLES);
            gl_grid_vertex_array.unbind();

            _gl_grid_pipeline.unuse();
        }

        // Lights
        const int light_type_none = 0;
        const int light_type_spot = 1;
        const int light_type_direct = 2;
        const int light_type_area = 3;
        const int light_type_point = 4;
        const GLsizeiptr light_t_size{ 128 };
        const int max_light_count = 32;
        const int light_count_size_aligned = 16; // Sizeof `u_light_count` in fragment shader with alignment considered
        GLsizeiptr light_count = 0;
        arcadia::gl_uniform_buffer gl_uniform_buffer{ light_count_size_aligned + light_t_size * max_light_count };
        auto [vertices, indices] = _create_unit_cube_mesh(); // Icon for lights
        arcadia::gl_vertex_array gl_icon_vertex_array{ vertices,indices };
        _gl_icon_pipeline.use();
        _gl_icon_pipeline.set_uniform("u_view_mat", camera_view_mat4);
        _gl_icon_pipeline.set_uniform("u_proj_mat", camera_proj_mat4);
        gl_icon_vertex_array.bind();
        for(const auto& [light] : _gl_render_unit_lights)
        {
            if(light_count > max_light_count)
            {
                throw too_many_lights{ std::format("The max light count is {}",max_light_count) };
            }

            arcadia::match<void>(
                light,
                [&](const arcadia::null_light& light)
            {},
                [&](const arcadia::spot_light& light)
            {
                GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
                gl_uniform_buffer.sub_data(base_offfset + 0, sizeof(int), &light_type_spot);
                float cosine_inner_cutoff_angle = glm::cos(light.cutoff_angles.x);
                float cosine_outer_cutoff_angle = glm::cos(light.cutoff_angles.y);
                gl_uniform_buffer.sub_data(base_offfset + 4, sizeof(float), &cosine_inner_cutoff_angle);
                gl_uniform_buffer.sub_data(base_offfset + 8, sizeof(float), &cosine_outer_cutoff_angle);
                gl_uniform_buffer.sub_data(base_offfset + 16, sizeof(glm::vec3), &light.position);
                gl_uniform_buffer.sub_data(base_offfset + 32, sizeof(glm::vec3), &light.direction);
                gl_uniform_buffer.sub_data(base_offfset + 48, sizeof(glm::vec3), &light.attenuation_coefs);
                gl_uniform_buffer.sub_data(base_offfset + 64, sizeof(glm::vec3), &light.color);
                gl_uniform_buffer.sub_data(base_offfset + 80, sizeof(glm::vec3), &light.ambient_strength);
                gl_uniform_buffer.sub_data(base_offfset + 96, sizeof(glm::vec3), &light.diffuse_strength);
                gl_uniform_buffer.sub_data(base_offfset + 112, sizeof(glm::vec3), &light.specular_strength);
                ++light_count;

                _gl_icon_pipeline.set_uniform("u_transform_mat", glm::translate(arcadia::mat4::identity(), light.position));
            },
                [&](const arcadia::direct_light& light)
            {
                GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
                gl_uniform_buffer.sub_data(base_offfset + 0, sizeof(int), &light_type_direct);
                gl_uniform_buffer.sub_data(base_offfset + 32, sizeof(glm::vec3), &light.direction);
                gl_uniform_buffer.sub_data(base_offfset + 64, sizeof(glm::vec3), &light.color);
                gl_uniform_buffer.sub_data(base_offfset + 80, sizeof(glm::vec3), &light.ambient_strength);
                gl_uniform_buffer.sub_data(base_offfset + 96, sizeof(glm::vec3), &light.diffuse_strength);
                gl_uniform_buffer.sub_data(base_offfset + 112, sizeof(glm::vec3), &light.specular_strength);
                ++light_count;
            },
                [&](const arcadia::area_light& light)
            {},
                [&](const arcadia::point_light& light)
            {
                GLintptr base_offfset = light_count_size_aligned + light_count * light_t_size;
                gl_uniform_buffer.sub_data(base_offfset + 0, sizeof(int), &light_type_point);
                gl_uniform_buffer.sub_data(base_offfset + 16, sizeof(glm::vec3), &light.position);
                gl_uniform_buffer.sub_data(base_offfset + 48, sizeof(glm::vec3), &light.attenuation_coefs);
                gl_uniform_buffer.sub_data(base_offfset + 64, sizeof(glm::vec3), &light.color);
                gl_uniform_buffer.sub_data(base_offfset + 80, sizeof(glm::vec3), &light.ambient_strength);
                gl_uniform_buffer.sub_data(base_offfset + 96, sizeof(glm::vec3), &light.diffuse_strength);
                gl_uniform_buffer.sub_data(base_offfset + 112, sizeof(glm::vec3), &light.specular_strength);
                ++light_count;

                _gl_icon_pipeline.set_uniform("u_transform_mat", glm::translate(arcadia::mat4::identity(), light.position));
            }
            );

            _gl_icon_pipeline.set_uniform("u_color", glm::vec3{ 1.f,1.f,1.f });
            gl_icon_vertex_array.draw_indices(GL_TRIANGLES);

        }
        gl_icon_vertex_array.unbind();
        _gl_icon_pipeline.unuse();
        gl_uniform_buffer.sub_data(0, sizeof(int), &light_count);
        gl_uniform_buffer.bind_buffer_base(0);

        // Draw with mesh pipeline
        _gl_mesh_pipeline.use();
        auto tex_uniform_index = 0;
        _gl_mesh_pipeline.set_uniform("u_view_mat", camera_view_mat4);
        _gl_mesh_pipeline.set_uniform("u_proj_mat", camera_proj_mat4);
        _gl_mesh_pipeline.set_uniform("u_view_pos", camera_position);
        for(auto& [uuid, gl_meshes] : _gl_render_unit_meshes_umap)
        {
            for(auto& [
                gl_vertex_array,
                    transform_mat4,
                    gl_texture2d_ambient,
                    gl_texture2d_diffuse,
                    gl_texture2d_specular
            ] : gl_meshes)
            {
                ARCADIA_GL_CALL(glViewport(0, 0, viewport_size.x, viewport_size.y));

                _gl_mesh_pipeline.set_uniform("u_transform_mat", transform_mat4);
                _gl_mesh_pipeline.set_uniform("u_normal_mat", glm::mat3{ glm::transpose(glm::inverse(transform_mat4)) });

                _gl_mesh_pipeline.set_uniform("u_material.ambient", tex_uniform_index);
                gl_texture2d_ambient.bind(tex_uniform_index++);

                _gl_mesh_pipeline.set_uniform("u_material.diffuse", tex_uniform_index);
                gl_texture2d_diffuse.bind(tex_uniform_index++);

                _gl_mesh_pipeline.set_uniform("u_material.specular", tex_uniform_index);
                gl_texture2d_specular.bind(tex_uniform_index++);

                _gl_mesh_pipeline.set_uniform("u_material.shininess", 32.f);

                gl_vertex_array.bind();
                gl_vertex_array.draw_indices(GL_TRIANGLES);
                gl_vertex_array.unbind();

                gl_texture2d_ambient.unbind();
                gl_texture2d_diffuse.unbind();
                gl_texture2d_specular.unbind();

            }
        }
        _gl_mesh_pipeline.unuse();

        // Draw with skybox pipeline
        if(_gl_render_unit_skybox_opt)
        {
            _gl_skybox_pipeline.use();
            auto& [gl_vertex_array, gl_cubemap] = *_gl_render_unit_skybox_opt;

            _gl_skybox_pipeline.set_uniform("u_skybox", 0);
            gl_cubemap.bind(0);

            _gl_skybox_pipeline.set_uniform("u_view_mat", glm::mat4{ glm::mat3{camera_view_mat4} });
            _gl_skybox_pipeline.set_uniform("u_proj_mat", camera_proj_mat4);

            gl_vertex_array.bind();
            ARCADIA_GL_CALL(glDepthFunc(GL_LEQUAL));
            gl_vertex_array.draw_indices(GL_TRIANGLES);
            ARCADIA_GL_CALL(glDepthFunc(GL_LESS));
            gl_vertex_array.unbind();

            _gl_skybox_pipeline.unuse();
        }

        gl_framebuffer.unbind();
    }

}

void arcadia::gl_renderer::reset()
{
    _gl_render_unit_cameras.clear();
    _gl_render_unit_lights.clear();
    _gl_render_unit_meshes_umap.clear();
    _submitted_meshes_uuid_set.clear();
    _gl_render_unit_skybox_opt.reset();
}

auto arcadia::gl_renderer::get_render_result_id(std::size_t index) const -> void*
{
    return reinterpret_cast<void*>(std::get<0>(_gl_render_unit_cameras.at(index)).get_gl_texture2d().get_gl_id());
}

void arcadia::gl_renderer::_assert_frame_in_build() const
{
    ARCADIA_ASSERT(_in_build && "Frame is not in build, did you call `prepare()`?");
}

void arcadia::gl_renderer::_assert_frame_not_in_build() const
{
    ARCADIA_ASSERT(!_in_build && "Frame is in build, did you call `finalize()`?");
}

auto arcadia::gl_renderer::_create_unit_cube_mesh() const -> std::pair<std::vector<arcadia::vertex>, std::vector<arcadia::mesh::index_type>>
{
    return std::make_pair(
        std::vector<arcadia::vertex>{
        arcadia::vertex{ glm::vec3{ -1, -1, 1 } },
            arcadia::vertex{ glm::vec3{ 1, -1, 1 } },
            arcadia::vertex{ glm::vec3{ 1, 1, 1 } },
            arcadia::vertex{ glm::vec3{ -1, 1, 1 } },
            arcadia::vertex{ glm::vec3{ -1, -1, -1 } },
            arcadia::vertex{ glm::vec3{ 1, -1, -1 } },
            arcadia::vertex{ glm::vec3{ 1, 1, -1 } },
            arcadia::vertex{ glm::vec3{ -1, 1, -1 } },
    },
        std::vector<arcadia::mesh::index_type>{
        // pos-z
        0, 1, 2, 2, 3, 0,
            // neg-z
            5, 4, 7, 7, 6, 5,
            // pos-x
            1, 5, 6, 6, 2, 1,
            // neg-x
            4, 0, 3, 3, 7, 4,
            // pos-y
            3, 2, 6, 6, 7, 3,
            //neg-y
            4, 5, 1, 1, 0, 4
    }
    );
}



