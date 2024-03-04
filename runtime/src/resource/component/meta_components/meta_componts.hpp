#pragma once

#include<string>

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"
#include"resource/component/component.hpp"

namespace arcadia
{
    struct ARCADIA_API uuid_compont: arcadia::component_base
    {
    public:
        inline uuid_compont():
            _uuid(arcadia::uuid::next_val())
        {}
        ~uuid_compont() = default;
    private:
        arcadia::uuid _uuid;
    };


    struct ARCADIA_API name_component: arcadia::component_base
    {
    public:
        using self_type = name_component;
    public:
        name_component() = default;
        inline name_component(const std::string& name):
            _name(name)
        {}
        name_component(const nlohmann::json& json);
        ~name_component() = default;
        auto to_json() const->nlohmann::json;

        [[nodiscard]]
        auto get() const -> const std::string&
        {
            return _name;
        }

        operator const std::string& () const
        {
            return get();
        }

    private:
        std::string _name{};
    };
}
