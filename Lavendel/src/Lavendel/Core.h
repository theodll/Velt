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

#define BIT(x) (1 << x)