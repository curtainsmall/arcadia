#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <deque>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "platform/opengl.hpp" // To prevent any other headers including `gl.h` before this one

#include "platform/glfw.hpp"
#include "platform/jolt.hpp"
#include "resource/entt.hpp"