#pragma once

#include<string>
#include<variant>

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{
    struct ARCADIA_API component_state_base
    {};

    template<class State>
    concept component_state_like = requires{
        std::derived_from<State, arcadia::component_state_base>;
    };

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

    struct ARCADIA_API component: arcadia::noncopyable
    {
    public:
        inline component(const arcadia::component_state state ={}):
            _state(state)
        {}
        ~component() = default;

        [[nodiscard]]
        inline auto state() const -> const arcadia::component_state&
        {
            return _state;
        }

        inline void state(const arcadia::component_state& state)
        {
            _state = state;
        }

        template<arcadia::component_state_like ComponentState, class ...Args>
        void state(Args&& ...args)
        {
            state(ComponentState(std::forward<Args>(args)...));
        }

    private:
        arcadia::component_state _state;
    };

    template<class Component>
    concept component_like = requires(const Component comp, const nlohmann::json json)
    {
        std::derived_from<Component, arcadia::component>;
        {
            comp.to_json()
        }->std::same_as<nlohmann::json>;
        {
            Component(json)
        };
    };
}
