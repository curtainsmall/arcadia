#pragma once

#include<string>
#include<unordered_map>

#include"core/base.hpp"

#define ARCADIA_META_STRUCT(name) struct ARCADIA_API name
#define ARCADIA_META_STRUCT_BEGIN \
{\
private


namespace arcadia
{
    struct ARCADIA_API meta
    {
    public:
        meta() = default;
        template<class Type>
        meta(const std::string& name):
            name(name),
            size(sizeof(Type)),
            alignment(alignof(Type))
        {}
        ~meta() = default;
    public:
        static std::unordered_map<std::string, arcadia::meta> meta_umap{};

        std::string name{};
        std::size_t size{};
        std::size_t alignment{};
    };
}
