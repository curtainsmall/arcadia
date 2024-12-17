#pragma once

#define ACDA_ASSERT(x) assert(x)
#define ACDA_UNREACHABLE(msg) ACDA_ASSERT(0 && msg)