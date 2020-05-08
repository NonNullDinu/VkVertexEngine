#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

#define vx_log_level_debug spdlog::level::debug
#define vx_log_level_info spdlog::level::info
#define vx_log_level_warn spdlog::level::warn
#define vx_log_level_error spdlog::level::err
#define vx_log_level_critical spdlog::level::critical

namespace Vertex
{

    class CoreLogger
    {
    public:
        CoreLogger(const CoreLogger&) = delete;

        inline static void Flush()
        {
            s_Logger->flush();
        }

        inline static void SetLevel(const spdlog::level::level_enum level)
        {
            s_Logger->set_level(level);
        }

        inline static std::shared_ptr<spdlog::logger>& Get()
        {
            return s_Logger;
        }

        inline static void Debug(const char* message)
        {
#if defined(VX_CONFIGURATION_DEBUG)
            s_Logger->debug(message);
#endif
        }

        inline static void Info(const char* message)
        {
            s_Logger->info(message);
        }

        inline static void Warn(const char* message)
        {
            s_Logger->warn(message);
        }

        inline static void Error(const char* message)
        {
            s_Logger->error(message);
        }

        inline static void Critical(const char* message)
        {
            s_Logger->critical(message);
        }

        template<typename First, typename... Rest>
        inline static void Debug(First message, Rest... params)
        {
#if defined(VX_CONFIGURATION_DEBUG)
            s_Logger->debug(message, params...);
#endif
        }

        template<typename First, typename... Rest>
        inline static void Info(First message, Rest... params)
        {
            s_Logger->info(message, params...);
        }

        template<typename First, typename... Rest>
        inline static void Warn(First message, Rest... params)
        {
            s_Logger->warn(message, params...);
        }

        template<typename First, typename... Rest>
        inline static void Error(First message, Rest... params)
        {
            s_Logger->error(message, params...);
        }

        template<typename First, typename... Rest>
        inline static void Critical(First message, Rest... params)
        {
            s_Logger->critical(message, params...);
        }

    private:
        CoreLogger() { Init(); }

        static CoreLogger s_Instance;

        static void Init();

    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
    };

    class VX_PUBLIC_API ClientLogger
    {
    public:
        ClientLogger(const ClientLogger&) = delete;

        inline static void SetLevel(const spdlog::level::level_enum level)
        {
            s_Logger->set_level(level);
        }

        inline static void Debug(const char* message)
        {
            s_Logger->debug(message);
        }

        inline static void Info(const char* message)
        {
            s_Logger->info(message);
        }

        inline static void Warn(const char* message)
        {
            s_Logger->warn(message);
        }

        inline static void Error(const char* message)
        {
            s_Logger->error(message);
        }

        inline static void Critical(const char* message)
        {
            s_Logger->critical(message);
        }

        template<typename First, typename... Rest>
        inline static void Debug(First message, Rest... params)
        {
            s_Logger->debug(message, params...);
        }

        template<typename First, typename... Rest>
        inline static void Info(First message, Rest... params)
        {
            s_Logger->info(message, params...);
        }

        template<typename First, typename... Rest>
        inline static void Warn(First message, Rest... params)
        {
            s_Logger->warn(message, params...);
        }

        template<typename First, typename... Rest>
        inline static void Error(First message, Rest... params)
        {
            s_Logger->error(message, params...);
        }

        template<typename First, typename... Rest>
        inline static void Critical(First message, Rest... params)
        {
            s_Logger->critical(message, params...);
        }

    private:
        ClientLogger() { Init(); }

        static ClientLogger s_Instance;

        static void Init();

    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
    };

}