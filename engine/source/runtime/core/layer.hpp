#pragma once

#include <chrono>
#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <vector>
#include <array>

#include "core/assert.hpp"
#include "core/event.hpp"
#include "core/exception.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    struct ACDA_API LayerInterface
    {
    public:
        using SelfType = LayerInterface;
    public:
        LayerInterface(std::string_view name = "layer");
        virtual ~LayerInterface() = default;

        LayerInterface(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        [[nodiscard]]
        auto GetLayerName() const -> std::string_view;

        virtual void OnEvent(EventBase& event) = 0;
        virtual void OnUpdate() = 0;

    private:
        std::string _Name{};
    };

    namespace Concepts
    {
        template<class T>
        concept Layer = std::derived_from<T, LayerInterface>;
    }

    struct ACDA_API LayerStack
    {
    public:
        using LayerVectorType = std::vector<std::shared_ptr<LayerInterface>>;
        using LayerLookupTableType = std::unordered_map<std::type_index, std::size_t>;
        using SelfType = LayerStack;
    public:
        static auto Instance() -> SelfType&;

        template<
            Concepts::Layer Layer,
            class ...Args
        >
        auto PushLayer(Args&& ...args) -> SelfType&
        {
            return PushLayer(std::make_shared<Layer>(std::forward<Args>(args)...));
        }
        template<
            Concepts::Layer Layer
        >
        auto PushLayer(std::shared_ptr<Layer>&& sptr) -> SelfType&
        {
            return PushLayer(_Layers.begin(), std::move(sptr));
        }
        template<
            Concepts::Layer Layer,
            class ...Args
        >
        auto PushLayer(LayerVectorType::const_iterator iter, Args&& ...args) -> SelfType&
        {
            return PushLayer(iter, std::make_shared<Layer>(std::forward<Args>(args)...));
        }
        template<
            Concepts::Layer Layer
        >
        auto PushLayer(LayerVectorType::const_iterator iter, std::shared_ptr<Layer>&& sptr) -> SelfType&
        {
            std::size_t index = std::distance(_Layers.cbegin(), iter);
            _Layers.emplace(
                iter,
                std::move(sptr)
            );
            for(auto& [type_index, value] : _LookupTable)
            {
                if(value >= index)
                {
                    value++;
                }
            }
            _LookupTable.try_emplace(
                typeid(Layer),
                index
            );
            return *this;
        }
        auto PopLayer() -> SelfType&;
        auto PopLayer(LayerVectorType::const_iterator iter) -> SelfType&;
        auto PopAllLayers() -> SelfType&;

        template<Concepts::Layer Layer>
        auto ContainsLayer() const -> bool
        {
            std::type_index type_index = static_cast<std::type_index>(typeid(Layer));
            return _LookupTable.contains(type_index);
        }

        template<Concepts::Layer ...Layers>
        auto ContainsMultipleLayers() const -> bool
        {
            return (_LookupTable.contains(static_cast<std::type_index>(typeid(Layers))) && ...);
        }

        template<Concepts::Layer Layer>
        auto GetLayerShared() -> std::shared_ptr<Layer>
        {
            ACDA_ASSERT(ContainsLayer<Layer>());

            std::type_index type_index = static_cast<std::type_index>(typeid(Layer));
            std::size_t index = _LookupTable.at(type_index);
            return std::static_pointer_cast<Layer>(_Layers.at(index));
        }

        template<Concepts::Layer ...Layers>
        auto GetMultipleLayersShared() -> std::tuple<std::shared_ptr<Layers>...>
        {
            ACDA_ASSERT(ContainsMultipleLayers<Layers...>());

            std::tuple<std::shared_ptr<Layers>...> result{};
            _GetMultipleLayersSharedHelper<0, Layers...>(result);
            return result;
        }

        auto GetSize() -> std::size_t;

        auto begin() -> LayerVectorType::const_iterator;
        auto end() -> LayerVectorType::const_iterator;
        auto rbegin() -> LayerVectorType::const_reverse_iterator;
        auto rend() -> LayerVectorType::const_reverse_iterator;

    private:
        LayerStack() = default;
        ~LayerStack() = default;

        template<std::size_t Index, Concepts::Layer ...Layers>
        void _GetMultipleLayersSharedHelper(std::tuple<std::shared_ptr<Layers>...>& tuple)
        {
            if constexpr(Index < sizeof...(Layers))
            {
                using LayerType = std::tuple_element_t<Index, std::tuple<std::shared_ptr<Layers>...>>::element_type;
                std::type_index type_index = static_cast<std::type_index>(typeid(LayerType));
                std::size_t index = _LookupTable.at(type_index);
                std::get<Index>(tuple) = std::static_pointer_cast<LayerType>(_Layers.at(index));
                _GetMultipleLayersSharedHelper<Index + 1, Layers...>(tuple);
            }
            return;
        }

    private:
        LayerVectorType _Layers{};
        LayerLookupTableType _LookupTable{};
    };
}