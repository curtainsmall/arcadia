#pragma once

#include<memory>

#include"core/application/application.hpp"
#include"core/layer/layer.hpp"

extern auto create_application_uptr() -> std::unique_ptr<arcadia::application>;

auto main(
    int argc,
    const char** argv
) -> int
{
    arcadia::layer_stack::instance()
        .push_layer_at<arcadia::application>(0, arcadia::create_application_uptr())
        .at<arcadia::application>(0)
        .run();

    return 0;
}
