#include "pch.hpp"
#include "graphic_api.hpp"

#include"core/conditional.hpp"

auto arcadia::graphic_api::directx::to_flatbuffers(const self_type& dx) -> serialization_type
{
    return {
        arcadia::version::to_flatbuffers(dx.version)
    };
}

auto arcadia::graphic_api::directx::from_flatbuffers(const serialization_type& flat_dx) -> self_type
{
    return {
        arcadia::version::from_flatbuffers(flat_dx.ver())
    };
}

auto arcadia::graphic_api::opengl::to_flatbuffers(const self_type& gl) -> serialization_type
{
    return {
        arcadia::version::to_flatbuffers(gl.version)
    };
}

auto arcadia::graphic_api::opengl::from_flatbuffers(const serialization_type& flat_gl) -> self_type
{
    return {
        arcadia::version::from_flatbuffers(flat_gl.ver())
    };
}

auto arcadia::graphic_api::vulkan::to_flatbuffers(const self_type& vk) -> serialization_type
{
    return {
        arcadia::version::to_flatbuffers(vk.version)
    };
}

auto arcadia::graphic_api::vulkan::from_flatbuffers(const serialization_type& flat_vk) -> self_type
{
    return {
        arcadia::version::from_flatbuffers(flat_vk.ver())
    };
}

ARCADIA_API auto arcadia::graphic_api::to_flatbuffers(flatbuffers::FlatBufferBuilder& builder, const arcadia::graphic_api::type& graphic_api) -> serialization_type
{
    return arcadia::match<serialization_type>(
        graphic_api,
        [&](const arcadia::graphic_api::opengl& gl) -> serialization_type
    {
        return serialization_type{
            arcadia::serialization::graphic_api::opengl,
            builder.CreateStruct(
                arcadia::graphic_api::opengl::to_flatbuffers(gl)
).Union()
        };
    },
        [&](const arcadia::graphic_api::directx& dx) -> serialization_type
    {
        return serialization_type{
            arcadia::serialization::graphic_api::directx,
            builder.CreateStruct(
                arcadia::graphic_api::directx::to_flatbuffers(dx)
).Union()
        };
    },
        [&](const arcadia::graphic_api::vulkan& vk) -> serialization_type
    {
        return serialization_type{
            arcadia::serialization::graphic_api::vulkan,
            builder.CreateStruct(
                arcadia::graphic_api::vulkan::to_flatbuffers(vk)
).Union()
        };
    }
    );
}

ARCADIA_API auto arcadia::graphic_api::from_flatbuffers(const serialization_type& flat_graphic_api) -> arcadia::graphic_api::type
{
    switch(flat_graphic_api.type)
    {
        case arcadia::serialization::graphic_api::opengl:
        {
            return arcadia::graphic_api::opengl::from_flatbuffers(
                *static_cast<const arcadia::serialization::opengl*>(flat_graphic_api.ptr)
            );
        }
        case arcadia::serialization::graphic_api::directx:
        {
            return arcadia::graphic_api::directx::from_flatbuffers(
                *static_cast<const arcadia::serialization::directx*>(flat_graphic_api.ptr)
            );
        }
        case arcadia::serialization::graphic_api::vulkan:
        {
            return arcadia::graphic_api::vulkan::from_flatbuffers(
                *static_cast<const arcadia::serialization::vulkan*>(flat_graphic_api.ptr)
            );
        }
        default:
        {
            return {};
        }
    }
}
