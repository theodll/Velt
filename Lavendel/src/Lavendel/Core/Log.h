#pragma once

#include <memory>
#include <string_view>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include "Core.h"

namespace Lavendel {

class LAVENDEL_API Log {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
        return s_CoreLogger;
    }

    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
        return s_ClientLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// Helper function to extract just the function name from __PRETTY_FUNCTION__
constexpr std::string_view extract_function_name(std::string_view pretty_function) {
    // Find the opening parenthesis
    size_t paren = pretty_function.find('(');
    if (paren == std::string_view::npos)
        return pretty_function;
    
    // Find the last space before the parenthesis (end of return type)
    size_t space = pretty_function.rfind(' ', paren);
    if (space == std::string_view::npos)
        space = 0;
    else
        space++; // Skip the space itself
    
    return pretty_function.substr(space, paren - space);
}

} // namespace Lavendel

// Helper macro to get pretty function name
#if defined(__GNUC__) || defined(__clang__)
    #define LV_FUNC_NAME ::Lavendel::extract_function_name(__PRETTY_FUNCTION__)
#elif defined(_MSC_VER)
    #define LV_FUNC_NAME ::Lavendel::extract_function_name(__FUNCSIG__)
#else
    #define LV_FUNC_NAME __func__
#endif

// Core logger macros
#define LV_CORE_TRACE(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetCoreLogger(); \
        if (logger) \
            logger->trace("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)

#define LV_CORE_INFO(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetCoreLogger(); \
        if (logger) \
            logger->info("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)

#define LV_CORE_WARN(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetCoreLogger(); \
        if (logger) \
            logger->warn("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)

#define LV_CORE_ERROR(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetCoreLogger(); \
        if (logger) \
            logger->error("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)

#define LV_CORE_FATAL(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetCoreLogger(); \
        if (logger) \
            logger->critical("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)

// Client logger macros
#define LV_TRACE(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetClientLogger(); \
        if (logger) \
            logger->trace("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)

#define LV_INFO(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetClientLogger(); \
        if (logger) \
            logger->info("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)

#define LV_WARN(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetClientLogger(); \
        if (logger) \
            logger->warn("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)

#define LV_ERROR(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetClientLogger(); \
        if (logger) \
            logger->error("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)

#define LV_FATAL(...) \
    do { \
        auto& logger = ::Lavendel::Log::GetClientLogger(); \
        if (logger) \
            logger->critical("[{}] {}", LV_FUNC_NAME, fmt::format(__VA_ARGS__)); \
    } while(0)