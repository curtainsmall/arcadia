#pragma once

#include<memory>

#include"core/app/app.hpp"
#include"core/layer/layer.hpp"

extern auto create_application_uptr() -> std::unique_ptr<arcadia::app_layer>;

auto main(
    int argc,
    const char** argv
) -> int
{
    auto& layer_stack =  arcadia::layer_stack::instance();
    layer_stack.push_layer_at<arcadia::app_layer>(0, arcadia::create_application_uptr())
        .at<arcadia::app_layer>(0)
        .run();

    layer_stack.pop_all();
    return 0;
}
