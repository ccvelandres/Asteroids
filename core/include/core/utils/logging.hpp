#pragma once

/**
 * @file core/utils/logging.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @defgroup Logging
 * @brief Wrapper for logging implementation
 * @ingroup Utils
 * @{
 */

#include <spdlog/spdlog.h>
#include <exception>

#include "profiler.hpp"

/** @todo: add fallback macros to __func__, then replace L_TAG */

/** We use alias for spdlog in case we change it in the future */
namespace logging
{
    using namespace spdlog;

    /** @brief Convert typename to string */
    template <typename T>
    struct typename_to_string
    {
        static const char *get() { return typeid(T).name(); }
    };

    /** @brief Retrieves the base filename from __FILE__ */
    constexpr const char *baseFileName(const char *path)
    {
        const char *base = path;
        while (*path)
            (*path == '/' || *path == '\\') ? base = ++path : path++;
        return base;
    }
} // namespace logging

/** @brief Helper macro for overloading logging::typename_to_string */
#define L_TYPE_SETSTRING(TYPE, NAME)                  \
    namespace logging                                 \
    {                                                 \
        template <>                                   \
        struct typename_to_string<TYPE>               \
        {                                             \
            static const char *get() { return NAME; } \
        };                                            \
    }

/** @brief Converts TYPE to string. See logging::typename_to_string for overloading values */
#define L_TYPE_GETSTRING(TYPE) (logging::typename_to_string<TYPE>::get())

/** @brief Stringify @p STR */
#define L_STRING(STR) #STR

/**
 * @brief Add tags log calls in this scope (required for logging).
 * If the profiler is enabled, also adds a profiler block on the current scope
 */
#define L_TAG(STR)       \
    PROFILER_BLOCK(STR); \
    static const std::string tag__ { std::string(logging::baseFileName(__FILE__)) + std::string(": " STR) }

#if (CORE_LOG_ENABLE_TAG)
/** @brief Base macro for logging macros (See CORE_LOG_ENABLE_TAG to disable tags) */
#define L_LOG(level, STR, ...) logging::level("[{}] " STR, tag__, ##__VA_ARGS__)
#else
/** @brief Base macro for logging macros (See CORE_LOG_ENABLE_TAG to enable tags) */
#define L_LOG(level, STR, ...) logging::level(STR, ##__VA_ARGS__)
#endif

/** @brief Log macro for error level */
#define L_ERROR(STR, ...) L_LOG(error, STR, ##__VA_ARGS__)
/** @brief Log macro for warn level */
#define L_WARN(STR, ...)  L_LOG(warn, STR, ##__VA_ARGS__)
/** @brief Log macro for info level */
#define L_INFO(STR, ...)  L_LOG(info, STR, ##__VA_ARGS__)
/** @brief Log macro for debug level */
#define L_DEBUG(STR, ...) L_LOG(debug, STR, ##__VA_ARGS__)
/** @brief Log macro for trace level */
#define L_TRACE(STR, ...) L_LOG(trace, STR, ##__VA_ARGS__)

/** @brief Base macro for throwing exceptions */
#define L_THROW(EXCEPTION, STR, ...)     throw EXCEPTION(fmt::format("[{}] " STR, tag__, ##__VA_ARGS__))
/** @brief Throw macro for runtime_error exceptions */
#define L_THROW_RUNTIME(STR, ...) L_THROW(std::runtime_error, STR, ##__VA_ARGS__)
/** @brief Throw macro for logic_error exceptions */
#define L_THROW_LOGIC(STR, ...)   L_THROW(std::logic_error, STR, ##__VA_ARGS__)

/** @brief Assertion macro */
#define L_ASSERT(cond, msg, ...)                       \
    if (static_cast<bool>(cond) ? 0 : 1)               \
    {                                                  \
        L_THROW(std::logic_error, msg, ##__VA_ARGS__); \
    }

/** @brief Macro for unimplemented functions */
#define L_UNIMPLEMENTED(NAME)                      \
    {                                              \
        L_TAG(NAME);                               \
        L_THROW_RUNTIME("Unimplemented function"); \
    }

/** @} endgroup Logging */
