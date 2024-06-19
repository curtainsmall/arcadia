#include "pch.hpp"

#include "texture2d.hpp"

#include"boost/range/combine.hpp"

auto Texture2d::cascade(
    const Texture2d& texture2d_1,
    const Texture2d& texture2d_2,
    const std::function<glm::vec4(const glm::vec4&, const glm::vec4&)>& method
) -> Texture2d
{
    if(texture2d_1.size != texture2d_2.size)
    {
        throw IncompatibleTextureSize{ std::format("Size of texture2d 1 is {}x{}, while size of texture2d 2 is {}x{}",texture2d_1.size.x,texture2d_1.size.y,texture2d_2.size.x,texture2d_2.size.y) };
    }

    Texture2d res{};
    res.size = texture2d_1.size;
    for(const auto& [pixel_1, pixel_2] : boost::combine(texture2d_1.pixels, texture2d_2.pixels))
    {
        res.pixels.emplace_back(method(pixel_1, pixel_2));
    }
    return res;
}
