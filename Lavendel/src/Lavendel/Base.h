#pragma once


#ifdef LV_PLATFORM_WINDOWS
	#ifdef LV_BUILD_DLL
		#define LAVENDEL_API __declspec(dllexport)
	#else
		#define LAVENDEL_API __declspec(dllimport)
	#endif
#else
	#define LAVENDEL_API
#endif


namespace Lavendel {

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