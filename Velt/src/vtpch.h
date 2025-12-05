#pragma once

#ifdef vt_PLATFORM_WIN32
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
#include "Velt/Core/Log.h"
#include "Velt/Core/Core.h"
#include "Velt/Utils/Profiling/Instrumentor.h"