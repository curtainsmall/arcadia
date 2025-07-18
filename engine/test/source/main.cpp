#include "gtest/gtest.h"

#include "core/main.hpp"
#include "editor/editor_layer.hpp"

TEST(ArcadiaEditor, Overall)
{
    EXPECT_EQ(Arcadia::Main<Arcadia::EditorLayer>(), 0);
}
