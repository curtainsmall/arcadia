#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/file/file.hpp"
#include"core/layer/layer.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"function/window/window_events.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API project_layer: arcadia::layer_interface
    {
    public:
        project_layer();
        virtual ~project_layer();

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

        [[nodiscard]]
        inline auto has_project() const noexcept -> bool
        {
            return _project_sptr.get();
        }
    private:
        [[nodiscard]]
        auto _assert_and_get_scene() -> arcadia::scene&;

        void _save_project();
        void _load_project();

        void _on_window_should_close(arcadia::event::window_should_close& e);

        void _on_create_project(arcadia::event::create_project& e);
        void _on_open_project(arcadia::event::open_project& e);
        void _on_save_project(arcadia::event::save_project& e);
        void _on_save_project_as(arcadia::event::save_project_as& e);
        void _on_close_project(arcadia::event::close_project& e);

        void _on_create_scene(arcadia::event::create_scene& e);
        void _on_select_scene(arcadia::event::select_scene& e);
        void _on_close_scene(arcadia::event::close_scene& e);
        void _on_delete_scene(arcadia::event::delete_scene& e);

        void _on_new_entity(arcadia::event::new_entity& e);
        void _on_delete_entity(arcadia::event::delete_entity& e);

        void _on_add_component(arcadia::event::add_component& e);
        void _on_remove_component(arcadia::event::remove_component& e);

    private:
        std::filesystem::path _project_filepath{};
        std::shared_ptr<arcadia::project> _project_sptr{};

        std::shared_ptr<arcadia::renderer_interface> _renderer_sptr{};

        std::shared_ptr<arcadia::physics_simulator> _physics_simulator_sptr{};
    };
}
