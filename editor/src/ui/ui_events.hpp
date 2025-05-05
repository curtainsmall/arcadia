#pragma once

#include <memory>
#include <string>

#include "core/event.hpp"
#include "resource/entity_id.hpp"

namespace Arcadia
{
    namespace Events
    {
        class OpenImguiWindow: public EventBase
        {
        public:
            OpenImguiWindow(const std::string& id_string):
                IdString(id_string)
            {}
        public:
            const std::string IdString;
        };

        class ScaleImguiWindow: public EventBase
        {
        public:
            ScaleImguiWindow(float new_scale):
                NewScale(new_scale)
            {}
        public:
            const float NewScale;
        };

        // When keyboard input is occupied in multiple loops (e.g. text input)
        class KeyboardInputOccupied: public EventBase
        {
        public:
            KeyboardInputOccupied(bool occupied):
                Occupied(occupied)
            {}
        public:
            const bool Occupied;
        };

        //==== Events for physics simulator ====//

        class PhysicsSimulatorShouldUpdate: public EventBase
        {
        public:
            PhysicsSimulatorShouldUpdate(bool should_update):
                ShouldUpdate(should_update)
            {}
        public:
            const bool ShouldUpdate;
        };

        //==== Events for physics component ====//

        class PhysicsComponentNewBody: public EventBase
        {};

        //==== Events for modes ====//

        class TogglePlayMode: public EventBase
        {};

        //==== Events for viewport ====//

        class ShowGizmo: public EventBase
        {
        public:
            ShowGizmo(bool should_show_gizmo):
                ShouldShowGizmo(should_show_gizmo)
            {}
        public:
            const bool ShouldShowGizmo;
        };
    }
}