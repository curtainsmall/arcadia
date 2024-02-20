#include "pch.hpp"
#include "layer.hpp"

auto arcadia::layer_stack::instance() -> self_type&
{
    static self_type layer_stack{};
    return layer_stack;
}

auto arcadia::layer_stack::pop_layer() -> self_type&
{
    _layer_uptrs.erase(_layer_uptrs.end());
    return *this;
}

auto arcadia::layer_stack::pop_layer_at(std::size_t idx) -> self_type&
{
    if(idx >= _layer_uptrs.size())
    {
        throw out_of_range{ std::format("Index out of range: {}",idx) };
    }

    _layer_uptrs.erase(
        _layer_uptrs.begin() + idx
    );

    return *this;
}

auto arcadia::layer_stack::size() -> std::size_t
{
    return _layer_uptrs.size();
}

auto arcadia::layer_stack::begin() -> layer_uptr_vector_type::const_iterator
{
    return _layer_uptrs.begin();
}

auto arcadia::layer_stack::end() -> layer_uptr_vector_type::const_iterator
{
    return _layer_uptrs.end();
}

auto arcadia::layer_stack::rbegin() -> layer_uptr_vector_type::const_reverse_iterator
{
    return _layer_uptrs.rbegin();
}

auto arcadia::layer_stack::rend() -> layer_uptr_vector_type::const_reverse_iterator
{
    return _layer_uptrs.rend();
}
