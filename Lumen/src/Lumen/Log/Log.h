#pragma once
#include <memory>
#include "../Core.h"
#include "spdlog/spdlog.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace Lumen {

	class LUMEN_API Log
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

#define LM_CORE_CRITICAL(...)      ::Lumen::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define LM_CORE_ERROR(...)         ::Lumen::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LM_CORE_WARN(...)          ::Lumen::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LM_CORE_INFO(...)          ::Lumen::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LM_CORE_TRACE(...)         ::Lumen::Log::GetCoreLogger()->trace(__VA_ARGS__)

// CLIENT LOG MACROS
#define LM_CRITICAL(...)           ::Lumen::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LM_ERROR(...)              ::Lumen::Log::GetClientLogger()->error(__VA_ARGS__)
#define LM_WARN(...)               ::Lumen::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LM_INFO(...)		       ::Lumen::Log::GetClientLogger()->info(__VA_ARGS__)
#define LM_TRACE(...)		       ::Lumen::Log::GetClientLogger()->trace(__VA_ARGS__)