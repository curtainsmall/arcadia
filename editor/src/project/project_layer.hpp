#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"
#include"function/render/renderer.hpp"
#include"project/project.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API project_layer: arcadia::layer_interface
    {
    public:
        project_layer();
        virtual ~project_layer() = default;

        virtual auto on_event(const arcadia::event_base& event) -> bool override;
        virtual void on_update(delta_time_type delta_time) override;

        [[nodiscard]]
        inline auto has_project() const noexcept -> bool
        {
            return _project_uptr.get();
        }
    private:
        auto _on_new_project(const arcadia::event::new_project& e) -> bool;
        auto _on_save_project(const arcadia::event::save_project& e) -> bool;
        auto _on_save_project_as(const arcadia::event::save_project_as& e) -> bool;
        auto _on_close_project(const arcadia::event::close_project& e) -> bool;
    private:
        std::unique_ptr<arcadia::project> _project_uptr{};
        std::unique_ptr<arcadia::renderer_interface> _renderer_uptr{};
    };
}
