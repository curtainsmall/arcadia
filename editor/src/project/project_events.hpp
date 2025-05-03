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
    }
}