#include "pch.hpp"
#include "texture2d.hpp"

#include"boost/range/combine.hpp"

auto arcadia::texture2d::cascade(
    const texture2d& texture2d_1,
    const texture2d& texture2d_2,
    const std::function<glm::vec4(const glm::vec4&, const glm::vec4&)>& method
) -> texture2d
{
    if(texture2d_1.size != texture2d_2.size)
    {
        throw incompatible_texture_size{ std::format("Size of texture2d 1 is {}x{}, while size of texture2d 2 is {}x{}",texture2d_1.size.x,texture2d_1.size.y,texture2d_2.size.x,texture2d_2.size.y) };
    }

    arcadia::texture2d res{};
    res.size = texture2d_1.size;
    for(const auto& [pixel_1, pixel_2] : boost::combine(texture2d_1.pixels, texture2d_2.pixels))
    {
        res.pixels.emplace_back(method(pixel_1, pixel_2));
    }
    return res;
}
