#pragma once

#include<string>
#include<variant>

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"

#define ARCADIA_COMPONENT_TYPE_STR_GETERS(TypeStr) \
[[nodiscard]]\
static constexpr auto get_type_str_static() -> std::string\
{\
    return TypeStr;\
}\
[[nodiscard]]\
virtual inline auto get_type_str() const -> std::string override\
{\
    return get_type_str_static();\
}

namespace arcadia
{

    struct ARCADIA_API component_base: arcadia::noncopyable
    {
    public:
        virtual auto get_type_str() const->std::string = 0;

        [[nodiscard]]
        inline auto get_uuid() const -> arcadia::uuid
        {
            return _uuid;
        }
    private:
        arcadia::uuid _uuid{};
    };

    template<class Component>
    concept component_like = requires(const Component comp, const nlohmann::json json)
    {
        std::derived_from<Component, arcadia::component_base>;
        {
            Component::get_type_str_static()
        }->std::same_as<std::string>;

        {
            comp.to_json()
        }->std::same_as<nlohmann::json>;
        {
            Component(json)
        };
    };

}
