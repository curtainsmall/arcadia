#pragma once

#include <cassert>

#define ACDA_ASSERT(...) assert(__VA_ARGS__)
#define ACDA_UNREACHABLE(...) assert(false && __VA_ARGS__)