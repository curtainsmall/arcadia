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
        _layer_sptrs.erase(_layer_sptrs.begin());
    }
    return *this;
}

auto Arcadia::LayerStack::PopLayer(layer_sptr_vector_type::const_iterator iter) -> self_type&
{
    _layer_sptrs.erase(
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
    return _layer_sptrs.size();
}

auto Arcadia::LayerStack::begin() -> layer_sptr_vector_type::const_iterator
{
    return _layer_sptrs.begin();
}

auto Arcadia::LayerStack::end() -> layer_sptr_vector_type::const_iterator
{
    return _layer_sptrs.end();
}

auto Arcadia::LayerStack::rbegin() -> layer_sptr_vector_type::const_reverse_iterator
{
    return _layer_sptrs.rbegin();
}

auto Arcadia::LayerStack::rend() -> layer_sptr_vector_type::const_reverse_iterator
{
    return _layer_sptrs.rend();
}


