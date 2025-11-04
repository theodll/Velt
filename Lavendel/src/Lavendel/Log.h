#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Lavendel {

    class LAVENDEL_API Log
            {
                    public:
                    static void Init();

                    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
                    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
                    private:
                    static std::shared_ptr<spdlog::logger> s_CoreLogger;
                    static std::shared_ptr<spdlog::logger> s_ClientLogger;
            };

}

// Core log macros
#define LV_CORE_TRACE(...)    ::Lavendel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LV_CORE_INFO(...)     ::Lavendel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LV_CORE_WARN(...)     ::Lavendel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LV_CORE_ERROR(...)    ::Lavendel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LV_CORE_FATAL(...)    ::Lavendel::Log::GetCoreLogger()->assert(__VA_ARGS__)

// Client log macros
#define LV_TRACE(...)	      ::Lavendel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LV_INFO(...)	      ::Lavendel::Log::GetClientLogger()->info(__VA_ARGS__)
#define LV_WARN(...)	      ::Lavendel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LV_ERROR(...)	      ::Lavendel::Log::GetClientLogger()->error(__VA_ARGS__)
#define LV_FATAL(...)	      ::Lavendel::Log::GetClientLogger()->assert(__VA_ARGS__)