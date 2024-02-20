#pragma once

#include<memory>

#include"flatbuffers/flatbuffers.h"

#include"core/base.hpp"

namespace arcadia
{
    namespace serialization
    {
        using buffer_type = std::vector<std::byte>;

        template<class Data, class ...Args>
            requires requires (flatbuffers::FlatBufferBuilder builder, Data data, Args&& ...args)
        {
            {
                Data::to_flatbuffers(builder, data, std::forward<Args>(args)...)
            } -> std::same_as<flatbuffers::Offset<typename Data::serialization_type>>;
        }
        auto to_flatbuffers(const Data& data, Args&& ...args) -> buffer_type
        {
            flatbuffers::FlatBufferBuilder builder{};
            auto flat_data = Data::to_flatbuffers(
                builder,
                data,
                std::forward<Args>(args)...
            );
            builder.Finish(flat_data);

            buffer_type::value_type* pointer = builder.GetBufferPointer();
            buffer_type::size_type   len = builder.GetSize();
            return buffer_type{
                pointer,
                pointer + len
            };
        }

        template<class Data, class ...Args>
            requires requires(typename Data::serialization_type flat_data, Args&& ...args)
        {
            {
                Data::from_flatbuffers(flat_data, std::forward<Args>(args)...)
            } -> std::same_as<Data>;
        }
        auto from_flatbuffers(const buffer_type& buf, Args&& ...args) -> Data
        {
            return Data::from_flatbuffers(
                *flatbuffers::GetRoot<typename Data::serialization_type>(buf.data()),
                std::forward<Args>(args)...
            );
        }

        template<class Data, class ...Args>
            requires requires(typename Data::serialization_type flat_data, Args&& ...args)
        {
            {
                Data::from_flatbuffers_unique(flat_data, std::forward<Args>(args)...)
            } -> std::same_as<std::unique_ptr<Data>>;
        }
        auto from_flatbuffers_unique(const buffer_type& buf, Args&& ...args) -> std::unique_ptr<Data>
        {
            return Data::from_flatbuffers_unique(
                *flatbuffers::GetRoot<typename Data::serialization_type>(buf.data()),
                std::forward<Args>(args)...
            );
        }
    }
}
