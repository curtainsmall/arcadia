#pragma once

#include<string>

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"

#define ACDA_COMPONENT_TYPE_STR_GETTERS(type_str) \
[[nodiscard]]\
static constexpr auto GetTypeStrStatic() -> std::string\
{\
    return type_str;\
}\
[[nodiscard]]\
virtual auto GetTypeStr() const -> std::string override\
{\
    return GetTypeStrStatic();\
}

struct iComponent: Noncopyable
{
public:
    virtual auto GetTypeStr() const->std::string = 0;
};

template<class Component>
concept cComponent = requires(const Component comp, const nlohmann::json json)
{
    std::derived_from<Component, iComponent>;
    {
        Component::GetTypeStrStatic()
    }->std::same_as<std::string>;

    {
        comp.ToJson()
    }->std::same_as<nlohmann::json>;

    {
        Component(json)
    };
};
