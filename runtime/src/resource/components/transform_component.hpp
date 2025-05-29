#pragma once

#include "core/math.hpp"
#include "core/memento.hpp"
#include "core/nlohmann_json.hpp"
#include "platform/api_def.hpp"
#include "resource/components/component_interface.hpp"

namespace Arcadia
{
    enum struct ACDA_API TransformComponentFlags: std::uint8_t
    {
        None = 0,
        UseRotation = 0x01,
        UseDirection = 0x02,
        _EnumBitfield
    };

    struct ACDA_API TransformComponent:
        public ComponentInterface,
        public MementoOriginatorInterface
    {
    public:
        using SelfType = TransformComponent;
    private:
        struct ACDA_API _MementoData: public MementoDataBase
        {
        public:
            TransformComponentFlags Flags{ TransformComponentFlags::None };
            glm::vec3 Position{ Glm::Vec3_CreateZero() };
            glm::quat RotationQuaternion{ Glm::Quat_CreateIdentity() };
            glm::vec3 Direction{ Glm::Vec3_CreateUnitPositiveZ() };
            glm::vec3 Scale{ 1,1,1 };
            glm::vec3 Pivot{ Glm::Vec3_CreateZero() };
        };
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("transform");

        TransformComponent() = default;
        TransformComponent(const nlohmann::json& json);
        ~TransformComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        TransformComponent(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        [[nodiscard]]
        auto GetFlags() const->TransformComponentFlags;
        void SetFlags(TransformComponentFlags flags);
        [[nodiscard]]
        auto CheckFlag(TransformComponentFlags flag) const -> bool;
        void AddFlag(TransformComponentFlags flag);
        void RemoveFlag(TransformComponentFlags flag);

        [[nodiscard]]
        auto GetPosition() const -> const glm::vec3&;
        void SetPosition(const glm::vec3& vec);
        void IncreasePosition(const glm::vec3& vec);

        [[nodiscard]]
        auto GetRotationEularAngle() const -> const glm::vec3&;
        void SetRotationEularAngle(const glm::vec3& vec);
        void IncreaseRotationEularAngle(const glm::vec3& vec);
        [[nodiscard]]
        auto GetRotationQuaternion() const -> const glm::quat&;
        void SetRotationQuaternion(const glm::quat& quat);
        void IncreaseRotationQuaternion(const glm::quat& quat);

        [[nodiscard]]
        auto GetDirection() const -> const glm::vec3&;
        void SetDirection(const glm::vec3& vec);
        void IncreaseDirection(const glm::vec3& vec);

        [[nodiscard]]
        auto GetScale() const -> const glm::vec3&;
        void SetScale(const glm::vec3& vec);
        void IncreaseScale(const glm::vec3& vec);

        [[nodiscard]]
        auto GetPivot() const -> const glm::vec3&;
        void SetPivot(const glm::vec3& vec);
        void IncreasePivot(const glm::vec3& vec);

        [[nodiscarc]]
        auto GetTransformMatrix() const -> const glm::mat4&;

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<MementoDataBase> override;
        virtual void OnRestore(const std::shared_ptr<MementoDataBase>& memento_data_base_sptr) override;

    private:
        TransformComponentFlags _Flags{ TransformComponentFlags::None };
        glm::vec3 _Position{ Glm::Vec3_CreateZero() };
        glm::vec3 _RotationEularAngle{ Glm::Vec3_CreateZero() };
        glm::quat _RotationQuaternion{ Glm::Quat_CreateIdentity() };
        glm::vec3 _Direction{ Glm::Vec3_CreateUnitPositiveZ() };
        glm::vec3 _Scale{ 1,1,1 };
        glm::vec3 _Pivot{ Glm::Vec3_CreateZero() };

        mutable bool _TransformMatrixDirty{ true };
        mutable glm::mat4 _TransformMatrix{ Glm::Mat4_CreateIdentity() };
    };
}