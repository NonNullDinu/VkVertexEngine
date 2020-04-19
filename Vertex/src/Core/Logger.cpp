#include "Logger.h"

namespace Vertex {

    Logger Logger::s_Instance;

    std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

    void Logger::Init()
    {
        s_CoreLogger = spdlog::stdout_color_mt("VX_CORE");
        s_ClientLogger = spdlog::stdout_color_mt("VX_CLIENT");

        s_CoreLogger->set_level(spdlog::level::debug);
        s_ClientLogger->set_level(spdlog::level::debug);
    }

}