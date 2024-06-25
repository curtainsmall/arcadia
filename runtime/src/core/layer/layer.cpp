#include "pch.hpp"

#include "layer.hpp"

iLayer::iLayer(const std::string& name):
    _name(name)
{}

auto iLayer::name() const -> const std::string&
{
    return _name;
}

auto LayerStack::instance() -> self_type&
{
    static self_type layer_stack{};
    return layer_stack;
}

auto LayerStack::pop_layer() -> self_type&
{
    if(size())
    {
        _layers.erase(_layers.begin());
    }
    return *this;
}

auto LayerStack::pop_layer(layer_vector_type::const_iterator iter) -> self_type&
{
    _layers.erase(
        iter
    );

    return *this;
}

auto LayerStack::pop_all() -> self_type&
{
    while(size())
    {
        pop_layer();
    }
    return *this;
}

auto LayerStack::size() -> size_t
{
    return _layers.size();
}

auto LayerStack::begin() -> layer_vector_type::const_iterator
{
    return _layers.begin();
}

auto LayerStack::end() -> layer_vector_type::const_iterator
{
    return _layers.end();
}

auto LayerStack::rbegin() -> layer_vector_type::const_reverse_iterator
{
    return _layers.rbegin();
}

auto LayerStack::rend() -> layer_vector_type::const_reverse_iterator
{
    return _layers.rend();
}


