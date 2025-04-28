#pragma once

#include <memory>
#include <vector>

#include "platform/api_def.hpp"

namespace Arcadia
{
    namespace Serialization
    {
        using BufferType = std::vector<std::byte>;

#if 0
        template<typename Data, typename ...Args>
            requires requires (flatbuffers::FlatBufferBuilder builder, Data data, Args&& ...args)
        {
            {
                Data::to_flatbuffers(builder, data, std::forward<Args>(args)...)
            } -> std::same_as<flatbuffers::Offset<typename Data::serialization_type>>;
        }
        auto to_flatbuffers(const Data& data, Args&& ...args) -> BufferType
        {
            flatbuffers::FlatBufferBuilder builder{};
            auto flat_data = Data::to_flatbuffers(
                builder,
                data,
                std::forward<Args>(args)...
            );
            builder.Finish(flat_data);

            BufferType::value_type* pointer = builder.GetBufferPointer();
            BufferType::size_type   len = builder.GetSize();
            return BufferType{
                pointer,
                pointer + len
            };
        }

        template<typename Data, typename ...Args>
            requires requires(typename Data::serialization_type flat_data, Args&& ...args)
        {
            {
                Data::from_flatbuffers(flat_data, std::forward<Args>(args)...)
            } -> std::same_as<Data>;
        }
        auto from_flatbuffers(const BufferType& buf, Args&& ...args) -> Data
        {
            return Data::from_flatbuffers(
                *flatbuffers::GetRoot<typename Data::serialization_type>(buf.data()),
                std::forward<Args>(args)...
            );
        }

        template<typename Data, typename ...Args>
            requires requires(typename Data::serialization_type flat_data, Args&& ...args)
        {
            {
                Data::from_flatbuffers_unique(flat_data, std::forward<Args>(args)...)
            } -> std::same_as<std::unique_ptr<Data>>;
        }
        auto from_flatbuffers_unique(const BufferType& buf, Args&& ...args) -> std::unique_ptr<Data>
        {
            return Data::from_flatbuffers_unique(
                *flatbuffers::GetRoot<typename Data::serialization_type>(buf.data()),
                std::forward<Args>(args)...
            );
        }
#endif
    }
}