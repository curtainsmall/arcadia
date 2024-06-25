#pragma once

#include<memory>

#include"core/event/event.hpp"

struct Project;
struct iRenderer;
struct PhysicsSimulator;
struct Scene;

namespace events
{

    //==== Event for project ====//

    ACDA_EVENT(
        ProjectBuilt,
        std::shared_ptr<Project>
    );
    ACDA_EVENT(
        ProjectUnbuilt
    );
    ACDA_EVENT(
        ProjectLoaded
    );
    ACDA_EVENT(
        ProjectSaved
    );

    //==== Event for renderer ====//

    ACDA_EVENT(
        RendererBuilt,
        std::shared_ptr<iRenderer>
    );
    ACDA_EVENT(
        RendererUnbuilt
    );

    //==== Event for physics simulator ====//

    ACDA_EVENT(
        PhysicsSimulatorBuilt,
        std::shared_ptr<PhysicsSimulator>
    );

    ACDA_EVENT(
        PhysicsSimulatorUnbuilt
    );

    //==== Event for scene ====//

    ACDA_EVENT(
        SceneBuilt,
        std::shared_ptr<Scene>
    );
    ACDA_EVENT(
        SceneActivated,
        std::shared_ptr<Scene>
    );
    ACDA_EVENT(
        SceneDeactivated
    );
    ACDA_EVENT(
        SceneUnbuilt
    );
}
