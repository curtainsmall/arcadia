#pragma once

#include <memory>
#include <string>

#include "core/event.hpp"
#include "resource/entity_id.hpp"

namespace Arcadia
{
    namespace Events
    {
        struct OpenImguiWindow: public EventBase
        {
        public:
            OpenImguiWindow(std::string_view id_string):
                IdString(id_string)
            {
            }
        public:
            const std::string IdString;
        };

        struct ScaleImguiWindow: public EventBase
        {
        public:
            ScaleImguiWindow(float new_scale):
                NewScale(new_scale)
            {
            }
        public:
            const float NewScale;
        };

        // When keyboard input is occupied in multiple loops (e.g. text input)
        struct KeyboardInputOccupied: public EventBase
        {
        public:
            KeyboardInputOccupied(bool occupied):
                Occupied(occupied)
            {
            }
        public:
            const bool Occupied;
        };

        //==== Events for physics simulator ====//

        struct PhysicsSimulatorShouldUpdate: public EventBase
        {
        public:
            PhysicsSimulatorShouldUpdate(bool should_update):
                ShouldUpdate(should_update)
            {
            }
        public:
            const bool ShouldUpdate;
        };

        //==== Events for physics component ====//

        struct PhysicsComponentNewBody: public EventBase
        {
        };

        //==== Events for modes ====//

        struct SetPlayMode: public EventBase
        {
        public:
            SetPlayMode(bool in_play_mode):
                InPlayMode(in_play_mode)
            {
            }
        public:
            const bool InPlayMode;
        };

        //==== Events for viewport ====//

        struct ShowGizmo: public EventBase
        {
        public:
            ShowGizmo(bool should_show_gizmo):
                ShouldShowGizmo(should_show_gizmo)
            {
            }
        public:
            const bool ShouldShowGizmo;
        };
    }
}