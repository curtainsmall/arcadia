#include "project.hpp"

auto arcadia::project::is_modified() const -> bool
{
    bool modified = _modified;
    for(const auto& [key, scene] : scene_umap)
    {
        modified = modified && scene.is_modified();
    }

    return modified;
}
