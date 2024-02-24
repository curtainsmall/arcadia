#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/mesh_component/mesh_component.hpp"
#include"resource/component/skybox_component/skybox_component.hpp"

namespace arcadia
{
    struct ARCADIA_API render_result_info
    {
    public:
        void* id;
        glm::uvec2 size;
    };

    struct ARCADIA_API renderer_interface: arcadia::noncopyable
    {
    public:
        using self_type = renderer_interface;
    public:
        virtual ~renderer_interface() = default;

        virtual void begin_frame() = 0;
        virtual void end_frame() = 0;

        virtual void submit(const arcadia::camera_component&) = 0;
        virtual void submit(const arcadia::mesh_component&) = 0;
        virtual void submit(const arcadia::skybox_component&) = 0;

        virtual void draw() = 0;

        virtual auto get_render_result_id(std::size_t index) const->void* = 0;

    };

    template<class Renderer>
    concept renderer_like = requires{
        std::derived_from<Renderer, arcadia::renderer_interface>;
    };

}
