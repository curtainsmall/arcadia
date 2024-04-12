#pragma once

#include<memory>

#include"core/event/event.hpp"

namespace Arcadia
{
    struct Project;
    struct iRenderer;
    struct PhysicsSimulator;
    struct Scene;

    namespace Event
    {

        //==== Event for project ====//

        ARCADIA_EVENT(
            ProjectBuilt,
            std::shared_ptr<Arcadia::Project>
        );
        ARCADIA_EVENT(
            ProjectUnbuilt
        );
        ARCADIA_EVENT(
            ProjectLoaded
        );
        ARCADIA_EVENT(
            ProjectSaved
        );

        //==== Event for renderer ====//

        ARCADIA_EVENT(
            RendererBuilt,
            std::shared_ptr<Arcadia::iRenderer>
        );
        ARCADIA_EVENT(
            RendererUnbuilt
        );

        //==== Event for physics simulator ====//

        ARCADIA_EVENT(
            PhysicsSimulatorBuilt,
            std::shared_ptr<Arcadia::PhysicsSimulator>
        );

        ARCADIA_EVENT(
            PhysicsSimulatorUnbuilt
        );

        //==== Event for scene ====//

        ARCADIA_EVENT(
            SceneBuilt,
            std::shared_ptr<Arcadia::Scene>
        );
        ARCADIA_EVENT(
            SceneActivated,
            std::shared_ptr<Arcadia::Scene>
        );
        ARCADIA_EVENT(
            SceneDeactivated
        );
        ARCADIA_EVENT(
            SceneUnbuilt
        );
    }
}
