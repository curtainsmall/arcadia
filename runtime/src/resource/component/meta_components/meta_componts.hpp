#pragma once

#include<string>

#include"core/base.hpp"
#include"core/uuid.hpp"
#include"resource/component/component.hpp"

namespace arcadia
{
    struct ARCADIA_API uuid_compont: arcadia::component
    {
    public:
        inline uuid_compont():
            _uuid(arcadia::uuid::next_val())
        {}
        ~uuid_compont() = default;
    private:
        arcadia::uuid _uuid;
    };

    struct ARCADIA_API tag_component: arcadia::component
    {
    public:
        tag_component() = default;
        inline tag_component(const std::string& msg):
            _msg(msg)
        {}
        ~tag_component() = default;

        [[nodiscard]]
        auto get() const -> const std::string&
        {
            return _msg;
        }

        operator const std::string& () const
        {
            return get();
        }

    private:
        std::string _msg{};
    };
}
