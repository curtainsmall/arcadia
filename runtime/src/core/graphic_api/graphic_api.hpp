#pragma once

#include<variant>

#include"flatbuffers_generated/meta_generated.h"

#include"core/base.hpp"
#include"core/version/version.hpp"

namespace arcadia
{
    namespace graphic_api
    {
        struct ARCADIA_API directx
        {
        public:
            using self_type = arcadia::graphic_api::directx;
            using serialization_type = arcadia::serialization::directx;
        public:
            static auto to_flatbuffers(const self_type& dx) -> serialization_type;
            static auto from_flatbuffers(const serialization_type& flat_dx) -> self_type;
        public:
            arcadia::version version{ 12,0,0 };

        };

        struct ARCADIA_API opengl
        {
        public:
            using self_type = arcadia::graphic_api::opengl;
            using serialization_type = arcadia::serialization::opengl;
        public:
            static auto to_flatbuffers(const self_type& gl) -> serialization_type;
            static auto from_flatbuffers(const serialization_type& flat_gl) -> self_type;
        public:
            arcadia::version version{ 4,6,0 };

        };

        struct ARCADIA_API vulkan
        {
        public:
            using self_type = arcadia::graphic_api::vulkan;
            using serialization_type = arcadia::serialization::vulkan;
        public:
            static auto to_flatbuffers(const self_type& vk) -> serialization_type;
            static auto from_flatbuffers(const serialization_type& flat_vk) -> self_type;
        public:
            arcadia::version version{ 1,3,0 };

        };

        using type = std::variant<
            arcadia::graphic_api::opengl,
            arcadia::graphic_api::directx,
            arcadia::graphic_api::vulkan
        >;

        struct ARCADIA_API serialization_type
        {
        public:
            inline serialization_type(
                arcadia::serialization::graphic_api type,
                flatbuffers::Offset<> offset
            ):
                type(type),
                offset(offset)
            {}

            inline serialization_type(
                arcadia::serialization::graphic_api type,
                const void* ptr
            ) :
                type(type),
                ptr(ptr)
            {}

        public:
            arcadia::serialization::graphic_api type;
            flatbuffers::Offset<> offset{};
            const void* ptr{ nullptr };

        };

        ARCADIA_API auto to_flatbuffers(flatbuffers::FlatBufferBuilder& builder, const arcadia::graphic_api::type& graphic_api) -> serialization_type;
        ARCADIA_API auto from_flatbuffers(const serialization_type& flat_graphic_api) -> arcadia::graphic_api::type;


    }
}
