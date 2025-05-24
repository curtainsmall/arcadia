#pragma once

#include "core/math.hpp"
#include "core/nlohmann_json.hpp"
#include "platform/api_def.hpp"
#include "resource/components/component_interface.hpp"

namespace Arcadia
{
    struct CameraComponent:
        public ComponentInterface
    {
    public:
        using SelfType = CameraComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("camera");

        [[nodiscard]]
        static auto GetUpAxis() -> const glm::vec3&;
        static void SetUpAxis(const glm::vec3& up_axis);

        CameraComponent() = default;
        CameraComponent(const nlohmann::json& json);
        ~CameraComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

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
        auto GetCursorMoveOffsetRange() const-> const glm::vec2&;
        void SetCursorMoveOffsetRange(const glm::vec2& range);

        [[nodiscard]]
        auto IsGridDisplaying() const -> bool;
        void SetGridDisplaying(bool displaying);

    #if 0
        auto MoveForward() -> SelfType&;
        auto MoveBackward() -> SelfType&;
        auto MoveLeft() -> SelfType&;
        auto MoveRight() -> SelfType&;
        auto MoveUp() -> SelfType&;
        auto MoveDown() -> SelfType&;
        auto Move(const glm::vec3& Offset) -> SelfType&;
        auto DragViewMove(const glm::vec2& Offset) -> SelfType&;

        auto RotateView(const glm::vec2& Offset) -> SelfType&;
        auto DragViewRotate(const glm::vec2& Offset) -> SelfType&;

        [[nodiscard]]
        auto GenerateViewMat4() const->glm::mat4;

        [[nodiscard]]
        auto GenerateProjectiveMat4() const->glm::mat4;

        [[nodiscard]]
        auto GenerateMat4(bool col_major = true) const->glm::mat4;

        // Get forward vector by position and target
        auto GetForwardDir() const->glm::vec3;
        auto GetLeftDir() const->glm::vec3;
        auto GetUpDir() const->glm::vec3;
    #endif

        static auto GenerateViewMat4(const glm::vec3& pos, const glm::vec3& dir) -> glm::mat4;

        auto GenerateProjectiveMat4() const->glm::mat4;

    #if 0
    private:
        // Angle of pitch
        // Look from right:
        // increase when rotating counter-clockwise
        auto _PitchAngle() const->float;

        // Angle of yaw
        // Look from top:
        // increase when rotating counter-clockwise
        auto _YawAngle() const->float;

        // Angle of roll
        // Lock from back:
        // increase when rotating counter-clockwise
        auto _RollAngle() const->float;

        // Test whether a cursor move should be filtered
        auto _TestCursorMove(float x_offset, float y_offset) -> bool;
    #endif

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