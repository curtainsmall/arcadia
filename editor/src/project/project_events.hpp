#pragma once

#include <memory>

#include "core/event.hpp"

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