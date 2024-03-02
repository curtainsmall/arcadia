#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/file/file.hpp"
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
        void _save_project();
        void _load_project();

        void _on_create_project(arcadia::event::create_project& e);
        void _on_open_project(arcadia::event::open_project& e);
        void _on_save_project(arcadia::event::save_project& e);
        void _on_save_project_as(arcadia::event::save_project_as& e);
        void _on_close_project(arcadia::event::close_project& e);

        void _on_create_scene(arcadia::event::create_scene& e);
        void _on_select_scene(arcadia::event::select_scene& e);
        void _on_delete_scene(arcadia::event::delete_scene& e);
    private:
        std::unique_ptr<arcadia::project> _project_uptr{};
        std::filesystem::path _project_filepath{};
        std::unique_ptr<arcadia::renderer_interface> _renderer_uptr{};
        std::size_t _project_hash{};
    };
}
