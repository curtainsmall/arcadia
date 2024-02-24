#pragma once

#include<algorithm>

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace arcadia
{
    struct ARCADIA_API degree
    {
    public:
        using self_type = arcadia::degree;
    public:
        explicit inline degree(float deg):
            _deg(deg)
        {}

        inline void set_deg(float deg)
        {
            _deg = deg;
        }
        inline void set_rad(float rad)
        {
            _deg = glm::degrees(rad);
        }
        inline auto operator=(float rad) -> self_type&
        {
            set_rad(rad);
            return *this;
        }

        [[nodiscard]]
        inline auto get_deg() const ->float
        {
            return _deg;
        }
        [[nodiscard]]
        inline auto get_rad() const -> float
        {
            return glm::radians(_deg);
        }
        inline operator float() const
        {
            return get_rad();
        }

        inline auto operator+(const self_type& rhs) const -> self_type
        {
            return self_type{ _deg + rhs._deg };
        }
        inline auto operator+=(const self_type& rhs) -> self_type&
        {
            _deg += rhs._deg;
            return *this;
        }
        inline auto operator-(const self_type& rhs) const -> self_type
        {
            return self_type{ _deg - rhs._deg };
        }
        inline auto operator-=(const self_type& rhs) -> self_type&
        {
            _deg -= rhs._deg;
            return *this;
        }
        inline auto operator*(float f) const -> self_type
        {
            return self_type{ _deg * f };
        }
        inline auto operator*=(float f) -> self_type&
        {
            _deg *= f;
            return *this;
        }
        inline auto operator/(float f) const -> self_type
        {
            return self_type{ _deg / f };
        }
        inline auto operator/=(float f) -> self_type&
        {
            _deg /= f;
            return *this;
        }
        auto operator<=>(const self_type& rhs) const = default;

        inline auto clamp(
            const self_type& min,
            const self_type& max
        ) -> bool
        {
            return std::clamp(_deg, min._deg, max._deg);
        }

    private:
        float _deg{ 0 };
    };
}
