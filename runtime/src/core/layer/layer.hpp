#pragma once

#include<chrono>
#include<format>
#include<memory>
#include<ranges>
#include<string>
#include<vector>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/exception.hpp"

namespace Arcadia
{
    struct iLayer: Arcadia::Noncopyable
    {
    public:
        using self_type = iLayer;
    public:
        iLayer(const std::string& name = "layer");
        virtual ~iLayer() = default;

        [[nodiscard]]
        auto GetName() const -> const std::string&
        {
            return _Name;
        }

        /// @brief Process event
        /// @param event Event to be processed
        virtual void OnEvent(Arcadia::EventBase& event) = 0;

        /// @brief Update layer
        virtual void OnUpdate() = 0;

    private:
        std::string _Name{};
    };

    template<class Layer>
    concept cLayer = requires{
        std::derived_from<Layer, Arcadia::iLayer>;
    };

    struct LayerStack
    {
    public:
        ARCADIA_EXCEPTION(OutOfRange);
        ARCADIA_EXCEPTION(EmptyStack);

        using layer_vector_type = std::vector<std::shared_ptr<iLayer>>;

        using self_type = LayerStack;
    public:
        static auto Instance() -> self_type&;

        template<
            Arcadia::cLayer Layer,
            class ...Args
        >
        auto PushLayer(Args&& ...args) -> self_type&
        {
            return PushLayer(std::make_shared<Layer>(std::forward<Args>(args)...));
        }
        template<
            Arcadia::cLayer Layer
        >
        auto PushLayer(std::shared_ptr<Layer>&& sptr) -> self_type&
        {
            _layers.emplace(_layers.begin(), std::move(sptr));
            return *this;
        }
        template<
            Arcadia::cLayer Layer,
            class ...Args
        >
        auto PushLayer(layer_vector_type::const_iterator iter, Args&& ...args) -> self_type&
        {
            return PushLayer(iter, std::make_shared<Layer>(std::forward<Args>(args)...));
        }
        template<
            Arcadia::cLayer Layer
        >
        auto PushLayer(layer_vector_type::const_iterator iter, std::shared_ptr<Layer>&& sptr) -> self_type&
        {
            _layers.emplace(
                iter,
                std::move(sptr)
            );
            return *this;
        }
        auto PopLayer() -> self_type&;
        auto PopLayer(layer_vector_type::const_iterator iter) -> self_type&;
        auto PopAll() -> self_type&;

        template<Arcadia::cLayer Layer = Arcadia::iLayer>
        auto At(std::size_t idx) -> Layer&
        {
            if(idx >= Size())
            {
                throw OutOfRange{ std::format("Index out of range: {}",idx) };
            }

            return static_cast<Layer&>(*_layers.at(Size() - idx - 1));
        }

        template<Arcadia::cLayer Layer = Arcadia::iLayer>
        auto Top() -> std::shared_ptr<Layer>
        {
            if(!Size())
            {
                throw EmptyStack{};
            }
            return std::static_pointer_cast<Layer>(_layers.front());
        }

        template<Arcadia::cLayer Layer = Arcadia::iLayer>
        auto Buttom() -> std::shared_ptr<Layer>
        {
            if(!Size())
            {
                throw EmptyStack{};
            }
            return std::static_pointer_cast<Layer>(_layers.back());
        }


        auto Size() -> std::size_t;

        auto begin() -> layer_vector_type::const_iterator;
        auto end() -> layer_vector_type::const_iterator;
        auto rbegin() -> layer_vector_type::const_reverse_iterator;
        auto rend() -> layer_vector_type::const_reverse_iterator;

    private:
        layer_vector_type _layers{};
    };
}
