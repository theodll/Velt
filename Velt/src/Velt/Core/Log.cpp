#include "vtpch.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Velt {

    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Log::s_FileSink;

    void Log::Init()
    {
        VT_PROFILE_FUNCTION();
        

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("%^[%T] %n: %v%$");

        s_FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Velt.log", true);
        s_FileSink->set_pattern("[%T] [%l] %n: %v");

        std::vector<spdlog::sink_ptr> coreSinks = { consoleSink, s_FileSink };
        s_CoreLogger = std::make_shared<spdlog::logger>("Velt", coreSinks.begin(), coreSinks.end());
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace); 
        spdlog::register_logger(s_CoreLogger);


        std::vector<spdlog::sink_ptr> clientSinks = { consoleSink, s_FileSink };
        s_ClientLogger = std::make_shared<spdlog::logger>("APP", clientSinks.begin(), clientSinks.end());
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace); 
        spdlog::register_logger(s_ClientLogger);

    }

    void Log::Flush()
    {
        if (s_CoreLogger)
            s_CoreLogger->flush();
        if (s_ClientLogger)
            s_ClientLogger->flush();
    }

    void Log::SetLogFile(const std::string& filepath)
    {
        s_FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filepath, true);
        s_FileSink->set_pattern("[%T] [%l] %n: %v");
    }


}