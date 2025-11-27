#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <cassert>

namespace Lavendel {

    class LAVENDEL_API Log
            {
                    public:
                    static void Init();

                    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
                        if (s_CoreLogger == nullptr)
                            assert("CoreLogger is null");
                        return s_CoreLogger; 
                    }
                    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
                        if (s_ClientLogger == nullptr)
                            assert("ClientLogger is null");
                        return s_ClientLogger; 
                }
                    private:
                    static std::shared_ptr<spdlog::logger> s_CoreLogger;
                    static std::shared_ptr<spdlog::logger> s_ClientLogger;
            };

}

// Core log macros with null checks
#define LV_CORE_TRACE(...)    if(::Lavendel::Log::GetCoreLogger()) ::Lavendel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LV_CORE_INFO(...)     if(::Lavendel::Log::GetCoreLogger()) ::Lavendel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LV_CORE_WARN(...)     if(::Lavendel::Log::GetCoreLogger()) ::Lavendel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LV_CORE_ERROR(...)    if(::Lavendel::Log::GetCoreLogger()) ::Lavendel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LV_CORE_FATAL(...)    if(::Lavendel::Log::GetCoreLogger()) ::Lavendel::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define LV_CORE_ASSERT(...)   if(::Lavendel::Log::GetCoreLogger()) ::Lavendel::Log::GetCoreLogger()->assert(__VA_ARGS__)

// Client log macros with null checks
#define LV_TRACE(...)	      if(::Lavendel::Log::GetClientLogger()) ::Lavendel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LV_INFO(...)	      if(::Lavendel::Log::GetClientLogger()) ::Lavendel::Log::GetClientLogger()->info(__VA_ARGS__)
#define LV_WARN(...)	      if(::Lavendel::Log::GetClientLogger()) ::Lavendel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LV_ERROR(...)	      if(::Lavendel::Log::GetClientLogger()) ::Lavendel::Log::GetClientLogger()->error(__VA_ARGS__)
#define LV_FATAL(...)	      if(::Lavendel::Log::GetClientLogger()) ::Lavendel::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LV_ASSERT(...).       if(::Lavendel::Log::GetClientLogger()) ::Lavendel::Log::GetClientLogger()->assert(__VA_ARGS__)