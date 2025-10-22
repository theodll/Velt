#pragma once
#include <memory>
#include "../Core.h"
#include "spdlog/spdlog.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace Lavendel {

	class LAVENDEL_API Log
	{
	public:
		Log();
		~Log();
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// CORE LOG MACROS
#define LV_CORE_CRITICAL(...)      ::Lavendel::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define LV_CORE_ERROR(...)         ::Lavendel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LV_CORE_WARN(...)          ::Lavendel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LV_CORE_INFO(...)          ::Lavendel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LV_CORE_TRACE(...)         ::Lavendel::Log::GetCoreLogger()->trace(__VA_ARGS__)

// CLIENT LOG MACROS
#define LV_CRITICAL(...)           ::Lavendel::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LV_ERROR(...)              ::Lavendel::Log::GetClientLogger()->error(__VA_ARGS__)
#define LV_WARN(...)               ::Lavendel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LV_INFO(...)		       ::Lavendel::Log::GetClientLogger()->info(__VA_ARGS__)
#define LV_TRACE(...)		       ::Lavendel::Log::GetClientLogger()->trace(__VA_ARGS__)