#include "Logger.h"

namespace Vertex {

    CoreLogger CoreLogger::s_Instance;
    std::shared_ptr<spdlog::logger> CoreLogger::s_Logger;

    void CoreLogger::Init()
    {
        s_Logger = spdlog::stdout_color_mt("VX_CORE");
        s_Logger->set_level(vx_log_level::debug);
    }

    ClientLogger::ClientLogger(const char* name, const vx_log_level_type level)
    {
        m_Logger = spdlog::stdout_color_mt(name);
        m_Logger->set_level(level);
    }

}