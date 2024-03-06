#pragma once

#include"flatbuffers_generated/window_generated.h"

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/layer/layer.hpp"
#include"core/math.hpp"
#include"function/window/glfw_header.hpp"
#include"function/window/window_events.hpp"
#include"platform/graphic_api/graphic_api.hpp"

namespace arcadia
{
    struct ARCADIA_API window_layer: arcadia::layer_interface
    {
    public:
        using self_type = arcadia::window_layer;
    public:
        window_layer(
            glm::ivec2 size ={ 800,600 },
            std::string title = "Untitled",
            int multisample_count = 0
        );
        virtual ~window_layer();

        [[nodiscard]]
        inline auto get_glfw_window_ptr() const -> GLFWwindow*
        {
            return _glfw_window_ptr;
        }

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update(delta_time_type delta_time) override;

        [[nodiscard]]
        auto get_title() const->const std::string&;
        auto set_title(const std::string& title) -> self_type&;

        [[nodiscard]]
        auto get_size() const->glm::ivec2;
        auto set_size(const glm::ivec2& size) -> self_type&;

        [[nodiscard]]
        auto get_size_state() const->arcadia::window_size_state;
        auto set_size_state(arcadia::window_size_state state) -> self_type&;

        [[nodiscard]]
        auto get_pos() const->glm::ivec2;
        auto set_pos(const glm::ivec2& pos) -> self_type&;

        [[nodiscard]]
        auto get_swap_interval() const -> int;
        auto set_swap_interval(int interval) -> self_type&;

        [[nodiscard]]
        auto get_visible() const -> bool;
        auto set_visible(bool visible) -> self_type&;

        [[nodiscard]]
        auto get_input_mode_cursor() const -> int;
        auto set_input_mode_cursor(int value) -> self_type&;

        [[nodiscard]]
        auto get_input_mode_sticky_keys() const -> int;
        auto set_input_mode_sticky_keys(int value) -> self_type&;

        [[nodiscard]]
        auto get_input_mode_sticky_mouse_buttons() const -> int;
        auto set_input_mode_sticky_mouse_buttons(int value) -> self_type&;

        [[nodiscard]]
        auto get_input_mode_lock_key_mods() const -> int;
        auto set_input_mode_lock_key_mods(int value) -> self_type&;

        [[nodiscard]]
        auto get_input_mode_raw_mouse_motion() const -> int;
        auto set_input_mode_raw_mouse_motion(int value) -> self_type&;

        auto get_multisample_count() const -> int;

    private:
        static inline auto _get_window_ptr_from_glfw_user_pointer(GLFWwindow* glfw_window_ptr) -> self_type*
        {
            return static_cast<self_type*>(glfwGetWindowUserPointer(glfw_window_ptr));
        }

        void _setup_callbacks();

        /// @brief Call [glfwSwapBuffers](https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14)
        void _swap_buffers();

        void _on_window_close_canceled(arcadia::event::window_close_canceled& e);

    private:
        static inline glfw_context _glfw_context{};

        std::string _title;
        int _swap_interval{ 0 };

        GLFWwindow* _glfw_window_ptr{ nullptr };
        glm::dvec2 _last_cursor_pos{ .0f };
        const int _multisample_count;
    };


}
