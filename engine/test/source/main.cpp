#include "gtest/gtest.h"

#include "core/main.hpp"
#include "editor/editor_setup.hpp"

TEST(ArcadiaEditor, Overall)
{
    EXPECT_EQ(Arcadia::Main(Arcadia::EditorSetup), 0);
}
