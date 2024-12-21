#pragma once

#include<memory>

#include"core/event/event.hpp"

namespace Arcadia
{
    class Project;
    class iRenderer;
    class PhysicsSimulator;
    class Scene;

    namespace Events
    {
        //==== Event for project ====//

        class ProjectBuilt: public EventBase
        {
        public:
            ProjectBuilt(const std::shared_ptr<Project>& project):
                Project(project)
            {}

        public:
            const std::shared_ptr<Project> Project;
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
            RendererBuilt(std::shared_ptr<iRenderer> renderer):
                Renderer(renderer)
            {}
        public:
            const std::shared_ptr<iRenderer> Renderer;
        };

        class RendererUnbuilt: public EventBase
        {};

        //==== Event for physics simulator ====//

        class PhysicsSimulatorBuilt: public EventBase
        {
        public:
            PhysicsSimulatorBuilt(const std::shared_ptr<PhysicsSimulator>& physcis_simulator):
                PhysicsSimulator(physcis_simulator)
            {}
        public:
            const std::shared_ptr<PhysicsSimulator> PhysicsSimulator;
        };

        class PhysicsSimulatorUnbuilt: public EventBase
        {};

        //==== Event for scene ====//

        class SceneBuilt: public EventBase
        {
        public:
            SceneBuilt(const std::shared_ptr<Scene>& scene):
                Scene(scene)
            {}
        public:
            const std::shared_ptr<Scene> Scene;
        };

        class SceneActivated: public EventBase
        {
        public:
            SceneActivated(const std::shared_ptr<Scene>& scene):
                Scene(scene)
            {}
        public:
            const std::shared_ptr<Scene> Scene;
        };

        class SceneDeactivated: public EventBase
        {};

        class Sceneunbuilt: public EventBase
        {};
    }
}