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
        _layer_sptrs.erase(_layer_sptrs.begin());
    }
    return *this;
}

auto arcadia::layer_stack::pop_layer(layer_sptr_vector_type::const_iterator iter) -> self_type&
{
    _layer_sptrs.erase(
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
    return _layer_sptrs.size();
}

auto arcadia::layer_stack::begin() -> layer_sptr_vector_type::const_iterator
{
    return _layer_sptrs.begin();
}

auto arcadia::layer_stack::end() -> layer_sptr_vector_type::const_iterator
{
    return _layer_sptrs.end();
}

auto arcadia::layer_stack::rbegin() -> layer_sptr_vector_type::const_reverse_iterator
{
    return _layer_sptrs.rbegin();
}

auto arcadia::layer_stack::rend() -> layer_sptr_vector_type::const_reverse_iterator
{
    return _layer_sptrs.rend();
}


