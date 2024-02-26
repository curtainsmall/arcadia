#include "project.hpp"

#include"core/file/file.hpp"

arcadia::project_layer::project_layer()
{}

arcadia::project_layer::~project_layer()
{}

auto arcadia::project_layer::on_event(const arcadia::event_base& event) -> bool
{
    return false;
}

void arcadia::project_layer::on_update(delta_time_type delta_time)
{}
