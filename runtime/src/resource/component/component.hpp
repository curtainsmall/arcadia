#pragma once

#include<string>
#include<variant>

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{
    struct ARCADIA_API component_state_ok
    {};
    struct ARCADIA_API component_state_error
    {
    public:
        component_state_error(const std::string& msg):
            msg(msg)
        {}
    public:
        std::string msg{};
    };

    using component_state = std::variant<
        arcadia::component_state_ok,
        arcadia::component_state_error
    >;

    struct ARCADIA_API component_base: arcadia::noncopyable
    {
    public:
        inline component_base(
            const arcadia::component_state state ={}
        ):
            _state(state)
        {}
        ~component_base() = default;

        [[nodiscard]]
        inline auto get_state() const -> const arcadia::component_state&
        {
            return _state;
        }

        inline void set_state(const arcadia::component_state& state)
        {
            _state = state;
        }

        template<class ComponentState, class ...Args>
        void set_state(Args&& ...args)
        {
            ARCADIA_ASSERT(std::holds_alternative<ComponentState>(_state));

            set_state(ComponentState(std::forward<Args>(args)...));
        }

    private:
        arcadia::component_state _state;
    };

    template<class Component>
    concept component_like = requires(const Component comp, const nlohmann::json json)
    {
        std::derived_from<Component, arcadia::component_base>;
        {
            comp.to_json()
        }->std::same_as<nlohmann::json>;
        {
            Component(json)
        };
    };

}
