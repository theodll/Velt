#pragma once

#include "Assert.h"
#include "Log.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


#if VT_PLATFORM_WINDOWS
	#ifdef VT_BUILD_DLL
		#define VELT_API __declspec(dllexport)
	#else
		#define VELT_API __declspec(dllimport)
	#endif
#else
	#define VELT_API
#endif


// Todo [21.02.26, Theo]: Move this to the cmake thing 
#define VT_COMPILE_VULKAN 1
#if VT_COMPILE_VULKAN
#define VT_VK_CHECK(f, e) if (!(f) == VK_SUCCESS) { VT_CORE_ASSERT(false, e); }
#endif

namespace Velt {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}


#define BIT(x) (1 << x)