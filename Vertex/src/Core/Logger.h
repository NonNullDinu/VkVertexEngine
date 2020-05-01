#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

#define vx_log_level spdlog::level
#define vx_log_level_type spdlog::level::level_enum

namespace Vertex {

    class CoreLogger
    {
    public:
        CoreLogger(const CoreLogger&) = delete;

        /// @todo: Create method implementations for each log level
        inline static std::shared_ptr<spdlog::logger> Get() { return s_Logger; }
        // ^ for now

    private:
        CoreLogger() { Init(); }

        static CoreLogger s_Instance;

        static void Init();

    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
    };

    /**
     * @todo: Improve the API. The goal is for the client to be able
     *      to do, for example, ClientLogger::Debug("{0}", args...);
     */
    class VX_PUBLIC_API ClientLogger
    {
    public:
        ClientLogger(const char* name, const vx_log_level_type level);

        /// @todo: Create method implementations for each log level
        inline std::shared_ptr<spdlog::logger>& Get() { return m_Logger; }
        // ^ for now

    private:
        std::shared_ptr<spdlog::logger> m_Logger;
    };

}