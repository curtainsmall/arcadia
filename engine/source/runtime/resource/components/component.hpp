#pragma once

#include <string>
#include <concepts>

#include "core/nlohmann_json.hpp"
#include "platform/defines.hpp"

#define ACDA_COMPONENT_TYPE_STR_GETTERS(type_str) \
[[nodiscard]]\
static constexpr auto GetTypeStringStatic() -> std::string_view\
{\
    return type_str;\
}\
[[nodiscard]]\
constexpr auto GetTypeString() const -> std::string_view \
{\
    return GetTypeStringStatic();\
}

namespace Arcadia
{
    namespace Concepts
    {
        template<class T>
        concept Component =
            !std::copyable<T>
            && requires(const T comp, const nlohmann::json json)
        {
            {
                T::GetTypeStringStatic()
            }->std::same_as<std::string_view>;

            {
                comp.GetTypeString()
            }->std::same_as<std::string_view>;

            {
                comp.ToJson()
            }->std::same_as<nlohmann::json>;

            T(json);
        };
    }
}