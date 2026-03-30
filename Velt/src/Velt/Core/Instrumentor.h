#pragma once

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>

#define VT_PROFILE_BEGIN_SESSION(name, filepath)
#define VT_PROFILE_END_SESSION()

#define VT_CONCAT_INTERNAL(x, y) x##y
#define VT_CONCAT(x, y) vt_CONCAT_INTERNAL(x, y)

#define VT_PROFILE_SCOPE(name) ZoneScopedN(name)

#ifdef _WIN32
#define VT_PROFILE_FUNCTION() ZoneScoped
#else
#define VT_PROFILE_FUNCTION() ZoneScoped
#endif

#else

#define VT_PROFILE_BEGIN_SESSION(name, filepath)
#define VT_PROFILE_END_SESSION()

#define VT_PROFILE_SCOPE(name)
#define VT_PROFILE_FUNCTION()

#endif