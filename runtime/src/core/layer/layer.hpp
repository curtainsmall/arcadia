#pragma once

#include<chrono>
#include<format>
#include<memory>
#include<ranges>
#include<string>
#include<vector>

#include"core/event/event.hpp"
#include"core/exception.hpp"
#include"core/noncopyable.hpp"
#include"platform/api_def.hpp"

namespace Arcadia
{
    class iLayer: public Noncopyable
    {
    public:
        using SelfType = iLayer;
    public:
        iLayer(const std::string& name = "layer");
        virtual ~iLayer() = default;

        [[nodiscard]]
        auto GetName() const -> const std::string&;

        /// @brief Process event
        /// @param event Event to be processed
        virtual void OnEvent(EventBase& event) = 0;

        /// @brief Update layer
        virtual void OnUpdate() = 0;

    private:
        std::string _Name{};
    };

    template<typename Layer>
    concept cLayer = requires{
        std::derived_from<Layer, iLayer>;
    };

    class LayerStack
    {
    public:
        ACDA_DEFINE_EXCEPTION(OutOfRange);
        ACDA_DEFINE_EXCEPTION(EmptyStack);

        using LayerVectorType = std::vector<std::shared_ptr<iLayer>>;

        using SelfType = LayerStack;
    public:
        static auto Instance() -> SelfType&;

        template<
            cLayer Layer,
            typename ...Args
        >
        auto PushLayer(Args&& ...args) -> SelfType&
        {
            return PushLayer(std::make_shared<Layer>(std::forward<Args>(args)...));
        }
        template<
            cLayer Layer
        >
        auto PushLayer(std::shared_ptr<Layer>&& sptr) -> SelfType&
        {
            _Layers.emplace(_Layers.begin(), std::move(sptr));
            return *this;
        }
        template<
            cLayer Layer,
            typename ...Args
        >
        auto PushLayer(LayerVectorType::const_iterator iter, Args&& ...args) -> SelfType&
        {
            return PushLayer(iter, std::make_shared<Layer>(std::forward<Args>(args)...));
        }
        template<
            cLayer Layer
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

        template<cLayer Layer = iLayer>
        auto GetLayer(std::size_t idx) -> Layer&
        {
            if(idx >= GetSize())
            {
                throw OutOfRange{ std::format("Index out of range: {}",idx) };
            }

            return static_cast<Layer&>(*_Layers.at(GetSize() - idx - 1));
        }

        template<cLayer Layer = iLayer>
        auto GetTopLayer() -> std::shared_ptr<Layer>
        {
            if(!GetSize())
            {
                throw EmptyStack{};
            }
            return std::static_pointer_cast<Layer>(_Layers.front());
        }

        template<cLayer Layer = iLayer>
        auto GetBottomLayer() -> std::shared_ptr<Layer>
        {
            if(!GetSize())
            {
                throw EmptyStack{};
            }
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