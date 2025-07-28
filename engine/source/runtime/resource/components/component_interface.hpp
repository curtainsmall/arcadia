#pragma once

#include <string>

#include "core/nlohmann_json.hpp"
#include "core/noncopyable.hpp"
#include "platform/defines.hpp"

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
    struct ACDA_API ComponentInterface:
        public Noncopyable
    {
    public:
        virtual ~ComponentInterface() = default;
        virtual auto GetTypeString() const -> std::string = 0;
    };

    namespace Concepts
    {
        template<class T>
        concept Component =
            std::derived_from<T, ComponentInterface>
            && requires(const T comp, const nlohmann::json json)
        {
            {
                T::GetTypeStringStatic()
            }->std::same_as<std::string>;

            {
                comp.ToJson()
            }->std::same_as<nlohmann::json>;

            T(json);
        };
    }
}