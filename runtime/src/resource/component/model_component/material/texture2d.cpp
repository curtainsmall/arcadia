#include "pch.hpp"
#include "texture2d.hpp"

#include"boost/range/combine.hpp"

auto Arcadia::Texture2d::Cascade(
    const Texture2d& texture2d_1,
    const Texture2d& texture2d_2,
    const std::function<glm::vec4(const glm::vec4&, const glm::vec4&)>& method
) -> Texture2d
{
    if(texture2d_1.Size != texture2d_2.Size)
    {
        throw IncompatibleTextureSize{ std::format("Size of texture2d 1 is {}x{}, while size of texture2d 2 is {}x{}",texture2d_1.Size.x,texture2d_1.Size.y,texture2d_2.Size.x,texture2d_2.Size.y) };
    }

    Arcadia::Texture2d res{};
    res.Size = texture2d_1.Size;
    for(const auto& [pixel_1, pixel_2] : boost::combine(texture2d_1.Pixels, texture2d_2.Pixels))
    {
        res.Pixels.emplace_back(method(pixel_1, pixel_2));
    }
    return res;
}
