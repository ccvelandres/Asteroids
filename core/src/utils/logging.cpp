#include <core/utils/logging.hpp>

#if defined(CONFIG_CORE_LOG_ENABLE)
#include <spdlog/spdlog.h>
#endif

namespace core::utils::logging
{
#if defined(CONFIG_CORE_LOG_ENABLE)
    using namespace spdlog;
#endif

    static level logLevel = []() -> level {
            /** static hack to set level on first call */
            spdlog::level::level_enum l = spdlog::level::level_enum(SPDLOG_LEVEL_OFF - CONFIG_CORE_LOG_LEVEL);
            spdlog::set_level(l);
            return level(CONFIG_CORE_LOG_LEVEL);
        }();

    void setPattern(const std::string &format)
    {
#if defined(CONFIG_CORE_LOG_ENABLE)
        spdlog::set_pattern(format);
#endif
    }

    void setLevel(level l)
    {
        logLevel = l;
    }

    void log(level l, const std::string &msg)
    {
        // skip when log level is below
        if( logLevel < l )
        {
            return;
        }

#if defined(CONFIG_CORE_LOG_ENABLE)
        switch(l)
        {
            case level::ERROR:
                spdlog::log(spdlog::level::err, msg);
                break;
            case level::CRITICAL:
                spdlog::log(spdlog::level::critical, msg);
                break;
            case level::WARN:
                spdlog::log(spdlog::level::warn, msg);
                break;
            case level::INFO:
                spdlog::log(spdlog::level::info, msg);
                break;
            case level::DEBUG:
                spdlog::log(spdlog::level::debug, msg);
                break;
            case level::TRACE:
                spdlog::log(spdlog::level::trace, msg);
                break;
            default:
                break;
        }
#endif
    }
}