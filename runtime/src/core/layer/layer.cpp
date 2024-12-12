#include "pch.hpp"

#include "layer.hpp"

iLayer::iLayer(const std::string& name) :
    _Name(name)
{}

auto iLayer::GetName() const -> const std::string&
{
    return _Name;
}

auto LayerStack::Instance() -> SelfType&
{
    static SelfType layer_stack{};
    return layer_stack;
}

auto LayerStack::PopLayer() -> SelfType&
{
    if(GetSize())
    {
        _Layers.erase(_Layers.begin());
    }
    return *this;
}

auto LayerStack::PopLayer(LayerVectorType::const_iterator iter) -> SelfType&
{
    _Layers.erase(
        iter
    );

    return *this;
}

auto LayerStack::PopAllLayers() -> SelfType&
{
    while(GetSize())
    {
        PopLayer();
    }
    return *this;
}

auto LayerStack::GetSize() -> size_t
{
    return _Layers.size();
}

auto LayerStack::begin() -> LayerVectorType::const_iterator
{
    return _Layers.begin();
}

auto LayerStack::end() -> LayerVectorType::const_iterator
{
    return _Layers.end();
}

auto LayerStack::rbegin() -> LayerVectorType::const_reverse_iterator
{
    return _Layers.rbegin();
}

auto LayerStack::rend() -> LayerVectorType::const_reverse_iterator
{
    return _Layers.rend();
}