#include "pch.hpp"
#include "layer.hpp"

arcadia::layer_interface::layer_interface(const std::string& name):
    _name(name)
{}

auto arcadia::layer_stack::instance() -> self_type&
{
    static self_type layer_stack{};
    return layer_stack;
}

auto arcadia::layer_stack::pop_layer() -> self_type&
{
    if(size())
    {
        _layer_uptrs.erase(_layer_uptrs.begin());
    }
    return *this;
}

auto arcadia::layer_stack::pop_layer(layer_uptr_vector_type::const_iterator iter) -> self_type&
{
    _layer_uptrs.erase(
        iter
    );

    return *this;
}

auto arcadia::layer_stack::pop_all() -> self_type&
{
    while(size())
    {
        pop_layer();
    }
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


