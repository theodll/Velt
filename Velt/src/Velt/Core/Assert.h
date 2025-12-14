#pragma once

#include "Core.h"
#include "Log.h"

#ifdef VT_PLATFORM_WINDOWS
#define VT_DEBUG_BREAK __debugbreak()
#elif defined(VT_COMPILER_CLANG)
#define VT_DEBUG_BREAK __builtin_debugtrap()
#else
#define VT_DEBUG_BREAK
#endif

#ifdef VT_DEBUG
#define VT_ENABLE_ASSERTS
#endif

#define VT_ENABLE_VERIFY

#ifdef VT_ENABLE_ASSERTS
#ifdef VT_COMPILER_CLANG
#define VT_CORE_ASSERT_MESSAGE_INTERNAL(...)  ::Velt::Log::PrintAssertMessage(::Velt::Log::Type::Core, "Assertion Failed", ##__VA_ARGS__)
#define VT_ASSERT_MESSAGE_INTERNAL(...)  ::Velt::Log::PrintAssertMessage(::Velt::Log::Type::Client, "Assertion Failed", ##__VA_ARGS__)
#else
#define VT_CORE_ASSERT_MESSAGE_INTERNAL(...)  ::Velt::Log::PrintAssertMessage(::Velt::Log::Type::Core, "Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
#define VT_ASSERT_MESSAGE_INTERNAL(...)  ::Velt::Log::PrintAssertMessage(::Velt::Log::Type::Client, "Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
#endif

#define VT_CORE_ASSERT(condition, ...) { if(!(condition)) { VT_CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); VT_DEBUG_BREAK; } }
#define VT_ASSERT(condition, ...) { if(!(condition)) { VT_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); VT_DEBUG_BREAK; } }
#else
#define VT_CORE_ASSERT(condition, ...)
#define VT_ASSERT(condition, ...)
#endif

#ifdef VT_ENABLE_VERIFY
#ifdef VT_COMPILER_CLANG
#define VT_CORE_VERIFY_MESSAGE_INTERNAL(...)  ::Velt::Log::PrintAssertMessage(::Velt::Log::Type::Core, "Verify Failed", ##__VA_ARGS__)
#define VT_VERIFY_MESSAGE_INTERNAL(...)  ::Velt::Log::PrintAssertMessage(::Velt::Log::Type::Client, "Verify Failed", ##__VA_ARGS__)
#else
#define VT_CORE_VERIFY_MESSAGE_INTERNAL(...)  ::Velt::Log::PrintAssertMessage(::Velt::Log::Type::Core, "Verify Failed" __VA_OPT__(,) __VA_ARGS__)
#define VT_VERIFY_MESSAGE_INTERNAL(...)  ::Velt::Log::PrintAssertMessage(::Velt::Log::Type::Client, "Verify Failed" __VA_OPT__(,) __VA_ARGS__)
#endif

#define VT_CORE_VERIFY(condition, ...) { if(!(condition)) { VT_CORE_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); VT_DEBUG_BREAK; } }
#define VT_VERIFY(condition, ...) { if(!(condition)) { VT_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); VT_DEBUG_BREAK; } }
#else
#define VT_CORE_VERIFY(condition, ...)
#define VT_VERIFY(condition, ...)
#endif