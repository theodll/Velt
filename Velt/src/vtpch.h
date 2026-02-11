#pragma once

#ifdef VT_PLATFORM_WIN32
#include <Windows.h>
#endif

// libs
#include <vulkan/vulkan.h>

// std
#include <string>
#include <vector>
#include <functional>
#include <array>
#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <set>
#include <stdexcept>
#include <cassert>
#include <unordered_set>
#include <thread>
#include <chrono>
#include <algorithm>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// self
#include "Velt/Core/Log.h"
#include "Velt/Core/Core.h"
#include "Velt/Core/Assert.h"
#include "Velt/Utils/Profiling/Instrumentor.h"