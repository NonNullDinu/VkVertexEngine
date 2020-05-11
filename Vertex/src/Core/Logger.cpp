#include "Logger.h"

namespace Vertex
{

    CoreLogger                      CoreLogger::s_Instance;
    std::shared_ptr<spdlog::logger> CoreLogger::s_Logger;

    void CoreLogger::Init()
    {
        s_Logger = spdlog::stdout_color_mt("VX_CORE");
        s_Logger->set_level(vx_log_level_debug);
    }

    ClientLogger                    ClientLogger::s_Instance;
    std::shared_ptr<spdlog::logger> ClientLogger::s_Logger;

    void ClientLogger::Init()
    {
        s_Logger = spdlog::stdout_color_mt("VX_CLIENT");
        s_Logger->set_level(vx_log_level_info);
    }

}