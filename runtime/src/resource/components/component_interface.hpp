#pragma once

#include<string>

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"

#define ACDA_COMPONENT_TYPE_STR_GETTERS(type_str) \
[[nodiscard]]\
static constexpr auto get_type_str_static() -> std::string\
{\
    return type_str;\
}\
[[nodiscard]]\
virtual auto get_type_str() const -> std::string override\
{\
    return get_type_str_static();\
}

struct iComponent: Noncopyable
{
public:
    virtual auto get_type_str() const->std::string = 0;
};

template<class Component>
concept cComponent = requires(const Component comp, const nlohmann::json json)
{
    std::derived_from<Component, iComponent>;
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
