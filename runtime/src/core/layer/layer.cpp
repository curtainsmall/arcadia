#include "pch.hpp"

#include "layer.hpp"

iLayer::iLayer(const std::string& name):
    _Name(name)
{}

auto LayerStack::Instance() -> self_type&
{
    static self_type layer_stack{};
    return layer_stack;
}

auto LayerStack::PopLayer() -> self_type&
{
    if(Size())
    {
        _layers.erase(_layers.begin());
    }
    return *this;
}

auto LayerStack::PopLayer(layer_vector_type::const_iterator iter) -> self_type&
{
    _layers.erase(
        iter
    );

    return *this;
}

auto LayerStack::PopAll() -> self_type&
{
    while(Size())
    {
        PopLayer();
    }
    return *this;
}

auto LayerStack::Size() -> std::size_t
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


