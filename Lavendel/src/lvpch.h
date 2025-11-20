#pragma once

#ifdef LV_PLATFORM_WIN32
#include <Windows.h>
#endif

// libs
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

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

// self
#include "Lavendel/Log.h"
#include "Lavendel/Core.h"
#include "Lavendel/Utils/Profiling/Instrumentor.h"