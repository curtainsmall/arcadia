#include "pch.hpp"

#include "layer.hpp"

Arcadia::iLayer::iLayer(const std::string& name):
    _Name(name)
{}

auto Arcadia::LayerStack::Instance() -> self_type&
{
    static self_type layer_stack{};
    return layer_stack;
}

auto Arcadia::LayerStack::PopLayer() -> self_type&
{
    if(Size())
    {
        _layers.erase(_layers.begin());
    }
    return *this;
}

auto Arcadia::LayerStack::PopLayer(layer_vector_type::const_iterator iter) -> self_type&
{
    _layers.erase(
        iter
    );

    return *this;
}

auto Arcadia::LayerStack::PopAll() -> self_type&
{
    while(Size())
    {
        PopLayer();
    }
    return *this;
}

auto Arcadia::LayerStack::Size() -> std::size_t
{
    return _layers.size();
}

auto Arcadia::LayerStack::begin() -> layer_vector_type::const_iterator
{
    return _layers.begin();
}

auto Arcadia::LayerStack::end() -> layer_vector_type::const_iterator
{
    return _layers.end();
}

auto Arcadia::LayerStack::rbegin() -> layer_vector_type::const_reverse_iterator
{
    return _layers.rbegin();
}

auto Arcadia::LayerStack::rend() -> layer_vector_type::const_reverse_iterator
{
    return _layers.rend();
}


