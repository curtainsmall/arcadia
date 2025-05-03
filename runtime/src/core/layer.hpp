#pragma once

#include <chrono>
#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

#include "core/assert.hpp"
#include "core/event.hpp"
#include "core/noncopyable.hpp"
#include "platform/api_def.hpp"

namespace Arcadia
{
    class LayerInterface: public Noncopyable
    {
    public:
        using SelfType = LayerInterface;
    public:
        LayerInterface(const std::string& name = "layer");
        virtual ~LayerInterface() = default;

        [[nodiscard]]
        auto GetName() const -> const std::string&;

        virtual void OnEvent(EventBase& event) = 0;
        virtual void OnUpdate() = 0;

    private:
        std::string _Name{};
    };

    namespace Concepts
    {
        template<typename T>
        concept Layer = std::derived_from<T, LayerInterface>;
    }

    class LayerStack
    {
    public:

        using LayerVectorType = std::vector<std::shared_ptr<LayerInterface>>;

        using SelfType = LayerStack;
    public:
        static auto Instance() -> SelfType&;

        template<
            Concepts::Layer Layer,
            typename ...Args
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
            _Layers.emplace(_Layers.begin(), std::move(sptr));
            return *this;
        }
        template<
            Concepts::Layer Layer,
            typename ...Args
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
            _Layers.emplace(
                iter,
                std::move(sptr)
            );
            return *this;
        }
        auto PopLayer() -> SelfType&;
        auto PopLayer(LayerVectorType::const_iterator iter) -> SelfType&;
        auto PopAllLayers() -> SelfType&;

        template<Concepts::Layer Layer = LayerInterface>
        auto GetLayer(std::size_t idx) -> Layer&
        {
            ACDA_ASSERT(idx >= GetSize(), "Index out of range");
            return static_cast<Layer&>(*_Layers.at(GetSize() - idx - 1));
        }

        template<Concepts::Layer Layer = LayerInterface>
        auto GetTopLayer() -> std::shared_ptr<Layer>
        {
            ACDA_ASSERT(GetSize(), "Empty layer stack");
            return std::static_pointer_cast<Layer>(_Layers.front());
        }

        template<Concepts::Layer Layer = LayerInterface>
        auto GetBottomLayer() -> std::shared_ptr<Layer>
        {
            ACDA_ASSERT(GetSize(), "Empty layer stack");
            return std::static_pointer_cast<Layer>(_Layers.back());
        }

        auto GetSize() -> std::size_t;

        auto begin() -> LayerVectorType::const_iterator;
        auto end() -> LayerVectorType::const_iterator;
        auto rbegin() -> LayerVectorType::const_reverse_iterator;
        auto rend() -> LayerVectorType::const_reverse_iterator;

    private:
        LayerVectorType _Layers{};
    };
}