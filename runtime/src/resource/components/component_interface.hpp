#pragma once

#include<string>

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"

#define ACDA_COMPONENT_TYPE_STR_GETTERS(type_str) \
[[nodiscard]]\
static constexpr auto GetTypeStringStatic() -> std::string\
{\
    return type_str;\
}\
[[nodiscard]]\
virtual auto GetTypeString() const -> std::string override\
{\
    return GetTypeStringStatic();\
}

namespace Arcadia
{
    class iComponent: public Noncopyable
    {
    public:
        virtual auto GetTypeString() const->std::string = 0;
    };

    template<typename Component>
    concept cComponent = requires(const Component comp, const nlohmann::json json)
    {
        std::derived_from<Component, iComponent>;
        {
            Component::GetTypeStringStatic()
        }->std::same_as<std::string>;

        {
            comp.ToJson()
        }->std::same_as<nlohmann::json>;

        {
            Component(json)
        };
    };
}