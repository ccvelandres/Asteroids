#pragma once

#include <spdlog/spdlog.h>
#include <exception>

#include "profiler.hpp"

/** We use alias for spdlog in case we change it in the future */
namespace logging
{
    using namespace spdlog;
}

/** Retrieves the base filename from __FILE__ */ 
constexpr const char *baseFileName( const char *path )
{
    const char *base = path;
    while ( *path )
        ( *path == '/' || *path == '\\' ) ? base = ++path : path++;
    return base;
}

/** Helper macros for appending the line and function */
#define L_STRING(STR) #STR
#define L_TAG(STR) \
    PROFILER_BLOCK(STR); \
    static const std::string tag__ { std::string(baseFileName(__FILE__)) + std::string(": " STR) }

#if (CORE_LOG_ENABLE_TAG)
#define L_LOG(level, STR, ...) logging::level("[{}] " STR, tag__, ##__VA_ARGS__)
#else
#define L_LOG(level, STR, ...) logging::level(STR, ##__VA_ARGS__)
#endif
#define L_ERROR(STR, ...) L_LOG(error, STR, ##__VA_ARGS__)
#define L_WARN(STR, ...) L_LOG(warn, STR, ##__VA_ARGS__)
#define L_INFO(STR, ...) L_LOG(info, STR, ##__VA_ARGS__)
#define L_DEBUG(STR, ...) L_LOG(debug, STR, ##__VA_ARGS__)
#define L_TRACE(STR, ...) L_LOG(trace, STR, ##__VA_ARGS__)

#define L_THROW(EX, STR, ...) throw EX(fmt::format("[{}] " STR, tag__, ##__VA_ARGS__))
#define L_THROW_RUNTIME(STR, ...) L_THROW(std::runtime_error, STR, ##__VA_ARGS__)
#define L_THROW_LOGIC(STR, ...) L_THROW(std::logic_error, STR, ##__VA_ARGS__)