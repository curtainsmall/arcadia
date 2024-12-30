#pragma once

#include<cassert>

#define _ACDA_ASSERT(condition) assert(condition)
#define _ACDA_ASSERT_WITH_MESSAGE(condition, message) _ACDA_ASSERT(condition && message)
#define _ACDA_GET_ASSERT_MACRO(_1, _2, name, ...) name
#define ACDA_ASSERT(...) _ACDA_GET_ASSERT_MACRO(__VA_ARGS__, _ACDA_ASSERT_WITH_MESSAGE, _ACDA_ASSERT)(__VA_ARGS__)

#define ACDA_UNREACHABLE(...) assert(false && __VA_ARGS__)