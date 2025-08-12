#pragma once

#include "platform/defines.hpp"
#include "resource/components/component.hpp"
#include "resource/cubemap.hpp"

namespace Arcadia
{
    struct ACDA_API SkyboxComponent:
        public Noncopyable
    {
    public:
        using SelfType = SkyboxComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("skybox");

        SkyboxComponent() = default;
        virtual ~SkyboxComponent() = default;

        [[nodiscard]]
        auto GetCubemap() const -> const Cubemap&;
        void SetCubemap(const Cubemap& cubemap);

    private:
        Cubemap _Cubemap{};
    };
}