#include "pch.hpp"
#include "gl_renderer.hpp"


arcadia::gl_renderer::gl_renderer(const std::filesystem::path& gl_shader_folder_path):
    _gl_mesh_pipeline(gl_shader_folder_path, arcadia::get_mesh_shaders_builder()),
    _gl_skybox_pipeline(gl_shader_folder_path, arcadia::get_skybox_shaders_builder())
{
    ARCADIA_GL_CALL(glEnable(GL_DEPTH_TEST));
    ARCADIA_GL_CALL(glEnable(GL_CULL_FACE));
}

void arcadia::gl_renderer::begin_frame()
{
    _check_frame_not_in_build_or_throw();
    _frame_in_build = true;

    _legacy_gl_render_unit_camera = true;
    _legacy_gl_render_unit_mesh = true;
}

void arcadia::gl_renderer::end_frame()
{
    _check_frame_in_build_or_throw();
    _frame_in_build = false;
}

void arcadia::gl_renderer::submit(const arcadia::camera_component& camera_comp)
{
    _check_frame_in_build_or_throw();

    if(_legacy_gl_render_unit_camera)
    {
        _gl_render_unit_cameras.clear();
        _legacy_gl_render_unit_camera = false;
    }

    _gl_render_unit_cameras.emplace_back(
        arcadia::gl_framebuffer{
            camera_comp.viewport_size,
            camera_comp.near_plane,
            camera_comp.far_plane
        },
        camera_comp.viewport_size,
        camera_comp.build_view_mat4(),
        camera_comp.build_proj_mat4()
    );
}

void arcadia::gl_renderer::submit(const arcadia::model_component& model_comp)
{
    _check_frame_in_build_or_throw();

    if(_legacy_gl_render_unit_mesh)
    {
        _gl_render_unit_meshes.clear();
        _legacy_gl_render_unit_mesh = false;
    }

    for(const auto& mesh : model_comp.get_meshes())
    {
        _gl_render_unit_meshes.emplace_back(
            arcadia::gl_vertex_array{ mesh.vertices, mesh.indices },
            arcadia::mat4::create_identity(),
            mesh.material.ambient_texture2d,
            mesh.material.diffuse_texture2d,
            mesh.material.specular_texture2d
        );
    }
}

void arcadia::gl_renderer::submit(const arcadia::skybox_component& skybox_comp)
{
    _check_frame_in_build_or_throw();

    const auto [vertices, indices] = _create_unit_cube_mesh();
    _gl_render_unit_skybox_opt.emplace(
        arcadia::gl_vertex_array{ vertices,indices },
        skybox_comp.cubemap
    );
}

void arcadia::gl_renderer::draw()
{
    _check_frame_not_in_build_or_throw();

    if(_gl_render_unit_cameras.empty())
    {
        throw draw_fail{ "No framebuffer to draw to" };
    }

    // For each framebuffer
    for(const auto& [gl_framebuffer, viewport_size, camera_view_mat4, camera_proj_mat4] : _gl_render_unit_cameras)
    {
        gl_framebuffer.bind();

        // Clear framebufers
        ARCADIA_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // Draw with mesh pipeline
        _gl_mesh_pipeline.use();

        // Set uniform for camera matrix
        _gl_mesh_pipeline.set_uniform_mat4("u_camera_view_mat", camera_view_mat4);
        _gl_mesh_pipeline.set_uniform_mat4("u_camera_proj_mat", camera_proj_mat4);

        // For each render unit mesh
        for(auto& [gl_vertex_array, transform_mat4, gl_texture2d_ambient, gl_texture2d_diffuse, gl_texture2d_specular] : _gl_render_unit_meshes)
        {
            ARCADIA_GL_CALL(glViewport(0, 0, viewport_size.x, viewport_size.y));

            _gl_mesh_pipeline.set_uniform_mat4("u_transform_mat", transform_mat4);

            _gl_mesh_pipeline.set_uniform_1i("u_tex_ambient", 0);
            gl_texture2d_ambient.bind(0);

            _gl_mesh_pipeline.set_uniform_1i("u_tex_diffuse", 1);
            gl_texture2d_diffuse.bind(1);

            _gl_mesh_pipeline.set_uniform_1i("u_tex_specular", 2);
            gl_texture2d_specular.bind(2);

            gl_vertex_array.bind();
            gl_vertex_array.draw_indices(GL_TRIANGLES);
            gl_vertex_array.unbind();

            gl_texture2d_ambient.unbind();
            gl_texture2d_diffuse.unbind();
            gl_texture2d_specular.unbind();

        }
        _gl_mesh_pipeline.unuse();

        // Draw with skybox pipeline
        if(_gl_render_unit_skybox_opt)
        {
            _gl_skybox_pipeline.use();
            auto& [gl_vertex_array, gl_cubemap] = *_gl_render_unit_skybox_opt;

            _gl_skybox_pipeline.set_uniform_1i("u_skybox", 0);
            gl_cubemap.bind(0);

            _gl_skybox_pipeline.set_uniform_mat4("u_camera_view_mat", glm::mat4{ glm::mat3{camera_view_mat4} });
            _gl_skybox_pipeline.set_uniform_mat4("u_camera_proj_mat", camera_proj_mat4);

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

void arcadia::gl_renderer::clear()
{
    _gl_render_unit_cameras.clear();
    _legacy_gl_render_unit_camera = false;

    _gl_render_unit_meshes.clear();
    _legacy_gl_render_unit_mesh = false;

    _gl_render_unit_skybox_opt.reset();
}

auto arcadia::gl_renderer::get_render_result_id(std::size_t index) const -> void*
{
    return reinterpret_cast<void*>(std::get<0>(_gl_render_unit_cameras.at(index)).get_gl_texture2d().get_gl_id());
}

void arcadia::gl_renderer::_check_frame_in_build_or_throw() const
{
    if(!_frame_in_build)
    {
        throw frame_not_in_build{ "Frame is not in build, did you call `begin_frame()`?" };
    }
}

void arcadia::gl_renderer::_check_frame_not_in_build_or_throw() const
{
    if(_frame_in_build)
    {
        throw frame_not_in_build{ "Frame is in build, did you call `end_frame()`?" };
    }
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
        1, 0, 3, 3, 2, 1,
            // neg-z
            4, 5, 6, 6, 7, 3,
            // pos-x
            5, 1, 2, 2, 6, 5,
            // neg-x
            0, 4, 7, 7, 3, 0,
            // pos-y
            7, 6, 2, 2, 3, 7,
            //neg-y
            0, 1, 5, 5, 4, 0
    }
    );
}
