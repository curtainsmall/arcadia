#pragma once

#include <memory>

#include "core/event/event.hpp"

namespace Arcadia
{
    class Project;
    class RendererInterface;
    class PhysicsSimulator;
    class Scene;

    namespace Events
    {
        //==== Event for project ====//

        class ProjectBuilt: public EventBase
        {
        public:
            ProjectBuilt(const std::shared_ptr<Project>& project_sptr):
                spProject(project_sptr)
            {}

        public:
            const std::shared_ptr<Project> spProject;
        };

        class ProjectUnbuilt: public EventBase
        {};

        class ProjectLoaded: public EventBase
        {};

        class ProjectSaved: public EventBase
        {};

        //==== Event for renderer ====//

        class RendererBuilt: public EventBase
        {
        public:
            RendererBuilt(std::shared_ptr<RendererInterface> renderer_sptr):
                spRenderer(renderer_sptr)
            {}
        public:
            const std::shared_ptr<RendererInterface> spRenderer;
        };

        class RendererUnbuilt: public EventBase
        {};

        //==== Event for physics simulator ====//

        class PhysicsSimulatorBuilt: public EventBase
        {
        public:
            PhysicsSimulatorBuilt(const std::shared_ptr<PhysicsSimulator>& physcis_simulator_sptr):
                spPhysicsSimulator(physcis_simulator_sptr)
            {}
        public:
            const std::shared_ptr<PhysicsSimulator> spPhysicsSimulator;
        };

        class PhysicsSimulatorUnbuilt: public EventBase
        {};

        //==== Event for scene ====//

        class SceneBuilt: public EventBase
        {
        public:
            SceneBuilt(const std::shared_ptr<Scene>& scene_sptr):
                spScene(scene_sptr)
            {}
        public:
            const std::shared_ptr<Scene> spScene;
        };

        class SceneActivated: public EventBase
        {
        public:
            SceneActivated(const std::shared_ptr<Scene>& scene_sptr):
                spScene(scene_sptr)
            {}
        public:
            const std::shared_ptr<Scene> spScene;
        };

        class SceneDeactivated: public EventBase
        {};

        class SceneUnbuilt: public EventBase
        {};
    }
}