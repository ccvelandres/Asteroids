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

#include <exception>
#include <cstddef>

#include "profiler.hpp"
#include <fmt/core.h>

namespace core::utils::logging
{
    enum class level
    {
        NONE,
        ERROR,
        CRITICAL,
        WARN,
        INFO,
        DEBUG,
        TRACE
    };

    /**
     * @brief Sets the logging format. 
     * See spdlog::set_format for format hints.
     * 
     * @param format format string
     */
    void setPattern(const std::string &format);

    /**
     * @brief Sets the logging level
     * 
     * @param l level
     */
    void setLevel(level l);

    /**
     * @brief log with specified level
     * 
     * @param l log level
     * @param msg message to log
     */
    void log(level l, const std::string &msg);

    template <typename... Args>
    inline void error(const std::string &s, const Args &...args)
    {
        log(level::ERROR, fmt::format(s, args...));
    }

    template <typename... Args>
    inline void critical(const std::string &s, const Args &...args)
    {
        log(level::CRITICAL, fmt::format(s, args...));
    }

    template <typename... Args>
    inline void warn(const std::string &s, const Args &...args)
    {
        log(level::WARN, fmt::format(s, args...));
    }

    template <typename... Args>
    inline void info(const std::string &s, const Args &...args)
    {
        log(level::INFO, fmt::format(s, args...));
    }

    template <typename... Args>
    inline void debug(const std::string &s, const Args &...args)
    {
        log(level::DEBUG, fmt::format(s, args...));
    }

    template <typename... Args>
    inline void trace(const std::string &s, const Args &...args)
    {
        log(level::TRACE, fmt::format(s, args...));
    }

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
} // namespace core::utils::logging

/** @todo: add fallback macros to __func__, then replace L_TAG */


/** @brief Helper macro for overloading core::utils::logging::typename_to_string */
#define L_TYPE_SETSTRING(TYPE, NAME)                  \
    namespace core::utils::logging                    \
    {                                                 \
        template <>                                   \
        struct typename_to_string<TYPE>               \
        {                                             \
            static const char *get() { return NAME; } \
        };                                            \
    }

/** @brief Converts TYPE to string. See core::utils::logging::typename_to_string for overloading values */
#define L_TYPE_GETSTRING(TYPE) (core::utils::logging::typename_to_string<TYPE>::get())

/** @brief Stringify @p STR */
#define L_STRING(STR) #STR
#define _L_CAT(A, B) A##B
#define L_CAT(A, B) _L_CAT(A,B)

/**
 * @brief Add tags log calls in this scope (required for logging).
 * If the profiler is enabled, also adds a profiler block on the current scope
 */
#define L_TAG(STR)       \
    PROFILER_BLOCK(STR); \
    static const std::string tag__ { std::string(core::utils::logging::baseFileName(__FILE__)) + std::string(": " STR) }

#if defined(CONFIG_CORE_LOG_LINENUM)
/** @brief Macro for adding line numbers to logs (See CONFIG_CORE_LOG_LINENUM to disable line numbers) */
#define __L_LINE_STR "[{:4}] "
/** @brief Macro for adding line numbers to logs (See CONFIG_CORE_LOG_LINENUM to disable line numbers) */
#define __L_LINE ,__LINE__
#else
/** @brief Macro for adding line numbers to logs (See CONFIG_CORE_LOG_LINENUM to enable line numbers) */
#define __L_LINE_STR
/** @brief Macro for adding line numbers to logs (See CONFIG_CORE_LOG_LINENUM to enable line numbers) */
#define __L_LINE
#endif

#if defined(CONFIG_CORE_LOG_TAG)
/** @brief Macro for adding tags to logs (See CONfiG_CORE_LOG_TAG to disable tags) */
#define __L_TAG_STR "[{}] "
/** @brief Macro for adding tags to logs (See CONfiG_CORE_LOG_TAG to disable tags) */
#define __L_TAG ,tag__
#else
/** @brief Macro for adding tags to logs (See CONfiG_CORE_LOG_TAG to enable tags) */
#define __L_TAG_STR
/** @brief Macro for adding tags to logs (See CONfiG_CORE_LOG_TAG to enable tags) */
#define __L_TAG
#endif

#if defined(CONFIG_CORE_LOG_ENABLE)
/** @brief Base macro for logging macros (See CONFIG_CORE_LOG_ENABLE_TAG to disable tags) */
#define L_LOG(level, STR, ...) core::utils::logging::level(__L_LINE_STR __L_TAG_STR STR __L_LINE __L_TAG, ##__VA_ARGS__)
/** @brief Base macro for throwing exceptions */
#define L_THROW(EXCEPTION, STR, ...) throw EXCEPTION(fmt::format(__L_LINE_STR __L_TAG_STR STR __L_LINE __L_TAG, ##__VA_ARGS__))
#else
/** @brief Base macro for logging macros (See CONFIG_CORE_LOG_ENABLE_TAG to enable tags) */
#define L_LOG(level, STR, ...)
/** @brief Base macro for throwing exceptions */
#define L_THROW(EXCEPTION, STR, ...) throw EXCEPTION(fmt::format(STR, ##__VA_ARGS__))
#endif

/** @brief Log macro for error level */
#define L_ERROR(STR, ...) L_LOG(error, STR, ##__VA_ARGS__)
/** @brief Log macro for warn level */
#define L_WARN(STR, ...) L_LOG(warn, STR, ##__VA_ARGS__)
/** @brief Log macro for info level */
#define L_INFO(STR, ...) L_LOG(info, STR, ##__VA_ARGS__)
/** @brief Log macro for debug level */
#define L_DEBUG(STR, ...) L_LOG(debug, STR, ##__VA_ARGS__)
/** @brief Log macro for trace level */
#define L_TRACE(STR, ...) L_LOG(trace, STR, ##__VA_ARGS__)

/** @brief Variable for holding rate limit state */
#define L_LOG_RATE_VAR __log_rate_limit##__func__
/** @brief Helper macro for starting rate limited logging. (End with L_LOG_RATE_END) */
#define L_LOG_RATE_START(RATE)            \
    {                                     \
        static size_t L_LOG_RATE_VAR = 0; \
        if (!(L_LOG_RATE_VAR++ % RATE))   \
        {
/** @brief Helper macro for starting rate limited logging. (Start with L_LOG_RATE_START) */
#define L_LOG_RATE_END \
    }                  \
    }

/** @brief Helper macro for rate limited logging macros */
#define L_LOG_RATE(RATE, level, STR, ...) \
    L_LOG_RATE_START(RATE);               \
    L_LOG(level, STR, ##__VA_ARGS__);     \
    L_LOG_RATE_END

/** @brief Rate limited log macro for error level */
#define L_ERROR_RATE(RATE, STR, ...) L_LOG_RATE(RATE, error, STR, ##__VA_ARGS__)
/** @brief Rate limited log macro for warn level */
#define L_WARN_RATE(RATE, STR, ...) L_LOG_RATE(RATE, warn, STR, ##__VA_ARGS__)
/** @brief Rate limited log macro for info level */
#define L_INFO_RATE(RATE, STR, ...) L_LOG_RATE(RATE, info, STR, ##__VA_ARGS__)
/** @brief Rate limited log macro for debug level */
#define L_DEBUG_RATE(RATE, STR, ...) L_LOG_RATE(RATE, debug, STR, ##__VA_ARGS__)
/** @brief Rate limited log macro for trace level */
#define L_TRACE_RATE(RATE, STR, ...) L_LOG_RATE(RATE, trace, STR, ##__VA_ARGS__)

/** @brief Throw macro for runtime_error exceptions */
#define L_THROW_RUNTIME(STR, ...) L_THROW(std::runtime_error, STR, ##__VA_ARGS__)
/** @brief Throw macro for logic_error exceptions */
#define L_THROW_LOGIC(STR, ...) L_THROW(std::logic_error, STR, ##__VA_ARGS__)

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
