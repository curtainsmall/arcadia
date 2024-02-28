#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"
#include"function/render/renderer.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API project_layer: arcadia::layer_interface
    {
    public:
        project_layer();
        virtual ~project_layer() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update(delta_time_type delta_time) override;

        [[nodiscard]]
        inline auto has_project() const noexcept -> bool
        {
            return _project_uptr.get();
        }
    private:
        void _on_create_project(arcadia::event::create_project& e);

        void _on_create_scene(arcadia::event::create_scene& e);
        void _on_select_scene(arcadia::event::select_scene& e);
        void _on_delete_scene(arcadia::event::delete_scene& e);
    private:
        std::unique_ptr<arcadia::project> _project_uptr{};
        std::unique_ptr<arcadia::renderer_interface> _renderer_uptr{};
    };
}
