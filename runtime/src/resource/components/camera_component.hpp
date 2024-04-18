#pragma once

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/components/component_interface.hpp"

namespace Arcadia
{
    struct ARCADIA_API CameraComponentMementoData: Arcadia::MementoDataBase
    {
    public:
        auto operator==(const CameraComponentMementoData&) const -> bool = default;
    public:
        glm::vec3  Position{ Arcadia::Vec3::PosZ() };
        glm::vec3  Target{ Arcadia::Vec3::Zero() };
        glm::vec3  Up{ Arcadia::Vec3::PosY() };
        float      NearPlane{ .1f };
        float      FarPlane{ 100.f };
        float      Fov{ 90.f };
        float      FovMin{ 1.f };
        float      FovMax{ 120.f };
        float      Speed{ .25f };
        glm::ivec2 ViewportSize{ 800,600 };
        bool       FixedUp{ true };
        float      UpEpsilon{ .1f };
        glm::vec2  CursorMoveOffsetRange{ -100.f,100.f };
        bool       ShouldDisplayGrid{ false };
    };

    struct ARCADIA_API CameraComponent:
        Arcadia::iComponent,
        Arcadia::iMementoOriginator
    {
    public:
        using self_type = CameraComponent;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("camera");

        CameraComponent() = default;
        CameraComponent(const nlohmann::json& json);
        ~CameraComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;


        auto MoveForward() -> self_type&;
        auto MoveBackward() -> self_type&;
        auto MoveLeft() -> self_type&;
        auto MoveRight() -> self_type&;
        auto MoveUp() -> self_type&;
        auto MoveDown() -> self_type&;
        auto Move(const glm::vec3& Offset) -> self_type&;
        auto DragViewMove(const glm::vec2& Offset) -> self_type&;

        auto RotateView(const glm::vec2& Offset) -> self_type&;
        auto DragViewRotate(const glm::vec2& Offset) -> self_type&;

        [[nodiscard]]
        auto GenerateViewMat4() const->glm::mat4;

        [[nodiscard]]
        auto GenerateProjMat4() const->glm::mat4;

        [[nodiscard]]
        auto GenerateMat4(bool col_major = true) const->glm::mat4;

        // Get forward vector by position and target
        auto GetForwardDir() const->glm::vec3;
        auto GetLeftDir() const->glm::vec3;
        auto GetUpDir() const->glm::vec3;

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<Arcadia::MementoDataBase> override;
        virtual void OnRestore(const std::shared_ptr<Arcadia::MementoDataBase>& sp_memento_data) override;

    private:
        // Angle of pitch
        // Look from right:
        // increase when rotating counter-clockwise
        auto _PitchAngle() const -> float;

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

    public:
        /// @brief Position of camera
        glm::vec3 Position{ Arcadia::Vec3::PosZ() };

        /// @brief Target that the camera pointing to
        glm::vec3 Target{ Arcadia::Vec3::Zero() };

        /// @brief Direction of global up
        /// @note This should always be unit vector
        glm::vec3 Up{ Arcadia::Vec3::PosY() };

        /// @brief Near plane of clip space
        float NearPlane{ .1f };

        /// @brief Far plane of clip space
        float FarPlane{ 100.f };

        /// @brief FOV angle in vertical direction
        float Fov{ 90.f };

        /// @brief Minimun value fo @ref Arcadia::camera::fov
        float FovMin{ 1.f };

        /// @brief Maximun value of @ref Arcadia::camera::fov
        float FovMax{ 120.f };

        /// @brief Move speed of free-camera
        float Speed{ .25f };

        /// @brief Size of the viewport of this camera
        glm::ivec2 ViewportSize{ 800,600 };

        /// @brief Whether @ref Arcadia::camera::up should be fixed
        bool FixedUp{ true };

        /// @brief How small angle between @ref Arcadia::camera::up and @ref Arcadia::camera::target can be
        float UpEpsilon{ .1f };

        /// @brief Cursor move offset that is out of this range will be silently ignored
        glm::vec2 CursorMoveOffsetRange{ -100.f,100.f };

        /// @brief Display a grid a X-Z plane
        bool ShouldDisplayGrid{ false };
    };
}
