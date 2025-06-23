

#include "layer.hpp"

Arcadia::LayerInterface::LayerInterface(const std::string& name):
    _Name(name)
{
}

auto Arcadia::LayerInterface::GetLayerName() const -> const std::string&
{
    return _Name;
}

auto Arcadia::LayerStack::Instance() -> SelfType&
{
    static SelfType layer_stack{};
    return layer_stack;
}

auto Arcadia::LayerStack::PopLayer() -> SelfType&
{
    if(GetSize())
    {
        _Layers.erase(_Layers.begin());
    }
    return *this;
}

auto Arcadia::LayerStack::PopLayer(LayerVectorType::const_iterator iter) -> SelfType&
{
    _Layers.erase(
        iter
    );

    return *this;
}

auto Arcadia::LayerStack::PopAllLayers() -> SelfType&
{
    while(GetSize())
    {
        PopLayer();
    }
    return *this;
}

auto Arcadia::LayerStack::GetSize() -> std::size_t
{
    return _Layers.size();
}

auto Arcadia::LayerStack::begin() -> LayerVectorType::const_iterator
{
    return _Layers.begin();
}

auto Arcadia::LayerStack::end() -> LayerVectorType::const_iterator
{
    return _Layers.end();
}

auto Arcadia::LayerStack::rbegin() -> LayerVectorType::const_reverse_iterator
{
    return _Layers.rbegin();
}

auto Arcadia::LayerStack::rend() -> LayerVectorType::const_reverse_iterator
{
    return _Layers.rend();
}