#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <cassert>

namespace Velt
{

    class VELT_API Log
    {

    public:
        static void Init();
        static void Flush();
        static void SetLogFile(const std::string &filepath);

        inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> s_FileSink;
    };

}

// Core log macros with null checks
#define VT_CORE_TRACE(...)            \
    if (::Velt::Log::GetCoreLogger()) \
    ::Velt::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VT_CORE_INFO(...)             \
    if (::Velt::Log::GetCoreLogger()) \
    ::Velt::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VT_CORE_WARN(...)             \
    if (::Velt::Log::GetCoreLogger()) \
    ::Velt::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VT_CORE_ERROR(...)            \
    if (::Velt::Log::GetCoreLogger()) \
    ::Velt::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VT_CORE_FATAL(...)            \
    if (::Velt::Log::GetCoreLogger()) \
    ::Velt::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define vt_CORE_ASSERT(...)           \
    if (::Velt::Log::GetCoreLogger()) \
    ::Velt::Log::GetCoreLogger()->assert(__VA_ARGS__)

// Client log macros with null checks
#define VT_TRACE(...)                   \
    if (::Velt::Log::GetClientLogger()) \
    ::Velt::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VT_INFO(...)                    \
    if (::Velt::Log::GetClientLogger()) \
    ::Velt::Log::GetClientLogger()->info(__VA_ARGS__)
#define VT_WARN(...)                    \
    if (::Velt::Log::GetClientLogger()) \
    ::Velt::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VT_ERROR(...)                   \
    if (::Velt::Log::GetClientLogger()) \
    ::Velt::Log::GetClientLogger()->error(__VA_ARGS__)
#define VT_FATAL(...)                   \
    if (::Velt::Log::GetClientLogger()) \
    ::Velt::Log::GetClientLogger()->critical(__VA_ARGS__)
#define VT_ASSERT(...) .if (::Velt::Log::GetClientLogger())::Velt::Log::GetClientLogger()->assert(__VA_ARGS__)