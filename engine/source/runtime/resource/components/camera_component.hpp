#pragma once

#include "core/math.hpp"
#include "core/memento.hpp"
#include "core/nlohmann_json.hpp"
#include "platform/defines.hpp"
#include "resource/components/component_interface.hpp"

namespace Arcadia
{
    struct CameraComponent_Memento
    {
    public:
        float NearPlane{ .1f };
        float FarPlane{ 100.f };
        float FovY{ glm::radians(75.f) };
        float FovYMin{ glm::radians(1.f) };
        float FovYMax{ glm::radians(120.f) };
        float Speed{ .25f };
        glm::i32vec2 ViewportSize{ 800,600 };
        bool UpAxisFixed{ true };
        float UpAxisAngleEpsilon{ glm::radians(0.1f) };
        glm::vec2 CursorMoveOffsetRange{ -100.f,100.f };
        bool GridDisplaying{ false };
    };

    struct ACDA_API CameraComponent:
        public ComponentInterface,
        public Mementoable<CameraComponent_Memento>
    {
    public:
        using MementoType = CameraComponent_Memento;
        using SelfType = CameraComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("camera");

        [[nodiscard]]
        static auto GetUpAxis() -> const glm::vec3&;
        static void SetUpAxis(const glm::vec3& up_axis);

        CameraComponent() = default;
        CameraComponent(const nlohmann::json& json);
        virtual ~CameraComponent() = default;
        [[nodiscard]]
        auto ToJson() const -> nlohmann::json;

        [[nodiscard]]
        auto GetNearPlane() const -> float;
        void SetNearPlane(float near_plane);

        [[nodiscard]]
        auto GetFarPlane() const -> float;
        void SetFarPlane(float far_plane);

        [[nodiscard]]
        auto GetFovY() const -> float;
        void SetFovY(float fov_y);

        [[nodiscard]]
        auto GetFovYMin() const -> float;
        void SetFovYMin(float fov_y_min);

        [[nodiscard]]
        auto GetFovYMax() const -> float;
        void SetFovYMax(float fov_y_max);

        [[nodiscard]]
        auto GetSpeed() const -> float;
        void SetSpeed(float speed);

        [[nodiscard]]
        auto GetViewportSize() const -> const glm::i32vec2&;
        void SetViewportSize(const glm::i32vec2& viewport_size);

        [[nodiscard]]
        auto IsUpAxisFixed() const -> bool;
        void SetUpAxisFixed(bool fixed);

        [[nodiscard]]
        auto GetUpAxisAngleEpsilon() const -> float;
        void SetUpAxisAngleEpsilon(float epsilon);

        [[nodiscard]]
        auto GetCursorMoveOffsetRange() const -> const glm::vec2&;
        void SetCursorMoveOffsetRange(const glm::vec2& range);

        [[nodiscard]]
        auto IsGridDisplaying() const -> bool;
        void SetGridDisplaying(bool displaying);

        static auto GenerateViewMat4(const glm::vec3& pos, const glm::vec3& dir) -> glm::mat4;

        auto GenerateProjectiveMat4() const -> glm::mat4;

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const -> std::unique_ptr<MementoType> override;
        virtual void OnRestore(const std::unique_ptr<MementoType>& memento_uptr) override;

    private:
        static inline glm::vec3 _UpAxis{ Glm::Vec3_CreateUnitPositiveY() };

        float _NearPlane{ .1f };
        float _FarPlane{ 100.f };
        float _FovY{ glm::radians(75.f) };
        float _FovYMin{ glm::radians(1.f) };
        float _FovYMax{ glm::radians(120.f) };
        float _Speed{ .25f };
        glm::i32vec2 _ViewportSize{ 800,600 };
        bool _UpAxisFixed{ true };
        float _UpAxisAngleEpsilon{ glm::radians(0.1f) };
        glm::vec2 _CursorMoveOffsetRange{ -100.f,100.f };
        bool _GridDisplaying{ false };
    };
}