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

        class NewProject: public EventBase
        {};

        class CreateProject: public EventBase
        {
        public:
            CreateProject(const std::string& name, const std::string& filepath_string):
                Name(name), FilepathString(filepath_string)
            {}
        public:
            const std::string Name;
            const std::string FilepathString;
        };

        class OpenProject: public EventBase
        {};

        class SaveProject: public EventBase
        {};

        class SaveProjectAs: public EventBase
        {};

        class CloseProject: public EventBase
        {};

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