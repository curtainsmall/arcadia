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

struct iLayer: Noncopyable
{
public:
    using self_type = iLayer;
public:
    iLayer(const std::string& name = "layer");
    virtual ~iLayer() = default;

    [[nodiscard]]
    auto name() const -> const std::string&;

    /// @brief Process event
    /// @param event Event to be processed
    virtual void on_event(EventBase& event) = 0;

    /// @brief Update layer
    virtual void on_update() = 0;

private:
    std::string _name{};
};

template<class Layer>
concept cLayer = requires{
    std::derived_from<Layer, iLayer>;
};

struct LayerStack
{
public:
    ACDA_EXCEPTION(OutOfRange);
    ACDA_EXCEPTION(EmptyStack);

    using layer_vector_type = std::vector<std::shared_ptr<iLayer>>;

    using self_type = LayerStack;
public:
    static auto instance() -> self_type&;

    template<
        cLayer Layer,
        class ...Args
    >
    auto push_layer(Args&& ...args) -> self_type&
    {
        return push_layer(std::make_shared<Layer>(std::forward<Args>(args)...));
    }
    template<
        cLayer Layer
    >
    auto push_layer(std::shared_ptr<Layer>&& sptr) -> self_type&
    {
        _layers.emplace(_layers.begin(), std::move(sptr));
        return *this;
    }
    template<
        cLayer Layer,
        class ...Args
    >
    auto push_layer(layer_vector_type::const_iterator iter, Args&& ...args) -> self_type&
    {
        return push_layer(iter, std::make_shared<Layer>(std::forward<Args>(args)...));
    }
    template<
        cLayer Layer
    >
    auto push_layer(layer_vector_type::const_iterator iter, std::shared_ptr<Layer>&& sptr) -> self_type&
    {
        _layers.emplace(
            iter,
            std::move(sptr)
        );
        return *this;
    }
    auto pop_layer() -> self_type&;
    auto pop_layer(layer_vector_type::const_iterator iter) -> self_type&;
    auto pop_all() -> self_type&;

    template<cLayer Layer = iLayer>
    auto at(std::size_t idx) -> Layer&
    {
        if(idx >= size())
        {
            throw OutOfRange{ std::format("Index out of range: {}",idx) };
        }

        return static_cast<Layer&>(*_layers.at(size() - idx - 1));
    }

    template<cLayer Layer = iLayer>
    auto top() -> std::shared_ptr<Layer>
    {
        if(!size())
        {
            throw EmptyStack{};
        }
        return std::static_pointer_cast<Layer>(_layers.front());
    }

    template<cLayer Layer = iLayer>
    auto buttom() -> std::shared_ptr<Layer>
    {
        if(!size())
        {
            throw EmptyStack{};
        }
        return std::static_pointer_cast<Layer>(_layers.back());
    }


    auto size() -> std::size_t;

    auto begin() -> layer_vector_type::const_iterator;
    auto end() -> layer_vector_type::const_iterator;
    auto rbegin() -> layer_vector_type::const_reverse_iterator;
    auto rend() -> layer_vector_type::const_reverse_iterator;

private:
    layer_vector_type _layers{};
};
