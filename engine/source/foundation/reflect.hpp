#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <nlohmann/json.hpp>

// One declaration of a component's fields, consumed by serialization, the
// inspector, undo and scripting.
//
// The macro must sit in the component's namespace: `ReflectFields` is looked up
// through argument dependent lookup.
//
//     ACDA_FIELDS(TransformComponent)
//     {
//         ACDA_FIELD_KEY(_Position, "position", Vec3);
//         ACDA_FIELD_KEY(_Scale,    "scale",    Vec3);
//     }

namespace Arcadia::Foundation
{
    enum class FieldKind
    {
        Bool = 0,
        Int,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Quat,
        String,
    };

    // Type-level replacement for the legacy "min == max == 0 means unbounded"
    // sentinel: a consumer cannot accidentally mean "clamp to zero" any more.
    struct FieldRange
    {
    public:
        bool Clamped{ false };
        float Min{ 0.f };
        float Max{ 0.f };

    public:
        [[nodiscard]] static constexpr auto Unbounded() -> FieldRange
        {
            return FieldRange{};
        }

        [[nodiscard]] static constexpr auto Closed(float min_value, float max_value) -> FieldRange
        {
            return FieldRange{ min_value < max_value, min_value, max_value };
        }

        [[nodiscard]] constexpr auto Clamps() const -> bool
        {
            return Clamped;
        }
    };

    // Strips the leading underscores of a member name ("_Position" -> "Position").
    [[nodiscard]] constexpr auto KeyOf(std::string_view name) -> std::string_view
    {
        while(!name.empty() && name.front() == '_')
        {
            name.remove_prefix(1);
        }
        return name;
    }

    struct Field
    {
    public:
        std::string_view Name{};
        std::string_view JsonKey{};
        FieldKind Kind{ FieldKind::Float };
        void* Data{ nullptr };
        FieldRange Range{};
        float Speed{ 1.f };
        std::string_view Format{ "%.3f" };

    public:
        // Human readable label: the member name without its leading underscore.
        [[nodiscard]] auto Label() const -> std::string_view
        {
            return KeyOf(Name);
        }
    };

    using FieldVisitor = std::function<void(Field&)>;

    // ---- json <-> single field -------------------------------------------------

    [[nodiscard]] inline auto ToJsonValue(const Field& field) -> nlohmann::json
    {
        switch(field.Kind)
        {
            case FieldKind::Bool:
            {
                return *static_cast<bool*>(field.Data);
            }
            case FieldKind::Int:
            {
                return *static_cast<std::int32_t*>(field.Data);
            }
            case FieldKind::Float:
            {
                return *static_cast<float*>(field.Data);
            }
            case FieldKind::Vec2:
            {
                const glm::vec2& vec = *static_cast<glm::vec2*>(field.Data);
                return nlohmann::json::array({ vec.x,vec.y });
            }
            case FieldKind::Vec3:
            {
                const glm::vec3& vec = *static_cast<glm::vec3*>(field.Data);
                return nlohmann::json::array({ vec.x,vec.y,vec.z });
            }
            case FieldKind::Vec4:
            {
                const glm::vec4& vec = *static_cast<glm::vec4*>(field.Data);
                return nlohmann::json::array({ vec.x,vec.y,vec.z,vec.w });
            }
            case FieldKind::Quat:
            {
                // Same layout as Glm::Quat_ToJson in the legacy code: (w, x, y, z).
                const glm::quat& quat = *static_cast<glm::quat*>(field.Data);
                return nlohmann::json::array({ quat.w,quat.x,quat.y,quat.z });
            }
            case FieldKind::String:
            {
                return *static_cast<std::string*>(field.Data);
            }
        }
        return nlohmann::json{};
    }

    inline void FromJsonValue(const Field& field, const nlohmann::json& json)
    {
        const auto read_components = [&json](float* out, std::size_t count) -> void
        {
            if(!json.is_array() || json.size() < count)
            {
                return;
            }
            for(std::size_t index = 0; index < count; ++index)
            {
                out[index] = json.at(index).get<float>();
            }
        };

        switch(field.Kind)
        {
            case FieldKind::Bool:
            {
                if(json.is_boolean())
                {
                    *static_cast<bool*>(field.Data) = json.get<bool>();
                }
                break;
            }
            case FieldKind::Int:
            {
                if(json.is_number_integer())
                {
                    *static_cast<std::int32_t*>(field.Data) = json.get<std::int32_t>();
                }
                break;
            }
            case FieldKind::Float:
            {
                if(json.is_number())
                {
                    *static_cast<float*>(field.Data) = json.get<float>();
                }
                break;
            }
            case FieldKind::Vec2:
            {
                read_components(&static_cast<glm::vec2*>(field.Data)->x, 2);
                break;
            }
            case FieldKind::Vec3:
            {
                read_components(&static_cast<glm::vec3*>(field.Data)->x, 3);
                break;
            }
            case FieldKind::Vec4:
            {
                read_components(&static_cast<glm::vec4*>(field.Data)->x, 4);
                break;
            }
            case FieldKind::Quat:
            {
                glm::quat& quat = *static_cast<glm::quat*>(field.Data);
                if(json.is_array() && json.size() >= 4)
                {
                    quat.w = json.at(0).get<float>();
                    quat.x = json.at(1).get<float>();
                    quat.y = json.at(2).get<float>();
                    quat.z = json.at(3).get<float>();
                }
                break;
            }
            case FieldKind::String:
            {
                if(json.is_string())
                {
                    *static_cast<std::string*>(field.Data) = json.get<std::string>();
                }
                break;
            }
        }
    }

    // ---- json <-> whole component ----------------------------------------------

    // Visits every declared field of `object`. `ReflectFields` is supplied by
    // ACDA_FIELDS and found through ADL.
    template<class T>
    void ForEachField(T& object, const FieldVisitor& visit)
    {
        ReflectFields(object, visit);
    }

    template<class T>
    [[nodiscard]] auto ToJson(const T& object) -> nlohmann::json
    {
        nlohmann::json json = nlohmann::json::object();
        // Fields are visited through their address, hence the const_cast on an
        // object the caller handed over as const.
        ForEachField(
            const_cast<T&>(object),
            [&json](Field& field) -> void
            {
                json[std::string(field.JsonKey)] = ToJsonValue(field);
            }
        );
        return json;
    }

    template<class T>
    void FromJson(T& object, const nlohmann::json& json)
    {
        ForEachField(
            object,
            [&json](Field& field) -> void
            {
                auto iter = json.find(std::string(field.JsonKey));
                if(iter != json.end())
                {
                    FromJsonValue(field, *iter);
                }
            }
        );
    }

    // ---- undo ------------------------------------------------------------------

    // Before/after images of one component; feeding them to a command stack is
    // enough to get undo, because a component's whole state is serializable.
    struct Patch
    {
    public:
        nlohmann::json Before{};
        nlohmann::json After{};

    public:
        [[nodiscard]] auto Changed() const -> bool
        {
            return Before != After;
        }
    };

    // Captures the "before" image on construction; MakePatch() pairs it with the
    // current state, so an edit only has to be wrapped in one scope.
    template<class T>
    struct EditScope
    {
    public:
        T& Object;
        nlohmann::json Before{};

    public:
        explicit EditScope(T& object):
            Object(object),
            Before(ToJson(object))
        {
        }

        [[nodiscard]] auto MakePatch() const -> Patch
        {
            return Patch{ Before, ToJson(Object) };
        }
    };
}

// ---- declaration macros --------------------------------------------------------

// Must be placed in the component's namespace.
#define ACDA_FIELDS(typeName)                                                                    \
    inline auto ReflectFields(typeName& Object, const ::Arcadia::Foundation::FieldVisitor& _visit) -> void

// JSON key defaults to the member name without its leading underscore.
#define ACDA_FIELD(member, kind)                                                                  \
    {                                                                                             \
        ::Arcadia::Foundation::Field _acda_field{                                                 \
            #member,                                                                              \
            ::Arcadia::Foundation::KeyOf(#member),                                                \
            ::Arcadia::Foundation::FieldKind::kind,                                               \
            static_cast<void*>(&Object.member)                                                   \
        };                                                                                        \
        _visit(_acda_field);                                                                      \
    }

// Explicit JSON key (needed when migrating components whose files already exist).
#define ACDA_FIELD_KEY(member, key, kind)                                                         \
    {                                                                                             \
        ::Arcadia::Foundation::Field _acda_field{                                                 \
            #member,                                                                              \
            (key),                                                                                \
            ::Arcadia::Foundation::FieldKind::kind,                                               \
            static_cast<void*>(&Object.member)                                                   \
        };                                                                                        \
        _visit(_acda_field);                                                                      \
    }

#define ACDA_FIELD_RANGE(member, kind, min_value, max_value)                                      \
    {                                                                                             \
        ::Arcadia::Foundation::Field _acda_field{                                                 \
            #member,                                                                              \
            ::Arcadia::Foundation::KeyOf(#member),                                                \
            ::Arcadia::Foundation::FieldKind::kind,                                               \
            static_cast<void*>(&Object.member),                                                  \
            ::Arcadia::Foundation::FieldRange::Closed((min_value), (max_value))                    \
        };                                                                                        \
        _visit(_acda_field);                                                                      \
    }

