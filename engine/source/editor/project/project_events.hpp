#pragma once

#include <memory>

#include "core/event.hpp"

namespace Arcadia
{
    struct Project;
    struct RendererInterface;
    struct PhysicsSimulator;
    struct Scene;

    namespace Events
    {
        //==== Event for project ====//

        struct NewProject: public EventBase
        {
        };

        struct CreateProject: public EventBase
        {
        public:
            CreateProject(const std::string& name, const std::string& filepath_string):
                Name(name), FilepathString(filepath_string)
            {
            }
        public:
            const std::string Name;
            const std::string FilepathString;
        };

        struct OpenProject: public EventBase
        {
        };

        struct SaveProject: public EventBase
        {
        };

        struct SaveProjectAs: public EventBase
        {
        };

        struct CloseProject: public EventBase
        {
        };

        struct ProjectBuilt: public EventBase
        {
        public:
            ProjectBuilt(const std::shared_ptr<Project>& project_sptr):
                spProject(project_sptr)
            {
            }

        public:
            const std::shared_ptr<Project> spProject;
        };

        struct ProjectUnbuilt: public EventBase
        {
        };

        struct ProjectLoaded: public EventBase
        {
        };

        struct ProjectSaved: public EventBase
        {
        };
    }
}