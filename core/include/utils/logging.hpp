#pragma once

#include <spdlog/spdlog.h>
#include <exception>

/** We use alias for spdlog in case we change it in the future */
namespace logging
{
    using namespace spdlog;
}

/** Helper macros for appending the line and function */
#define L_STRING(STR) #STR
#define L_TAG(STR) \
    static const std::string tag__ { STR }

#define L_LOG(level, STR, ...) logging::level("[{}] " STR, tag__, ##__VA_ARGS__)
#define L_ERROR(STR, ...) L_LOG(error, STR, ##__VA_ARGS__)
#define L_WARN(STR, ...) L_LOG(warn, STR, ##__VA_ARGS__)
#define L_INFO(STR, ...) L_LOG(info, STR, ##__VA_ARGS__)
#define L_DEBUG(STR, ...) L_LOG(debug, STR, ##__VA_ARGS__)
#define L_TRACE(STR, ...) L_LOG(trace, STR, ##__VA_ARGS__)

#define L_THROW_RUNTIME(STR, ...) throw std::runtime_error(fmt::format("[{}] " STR, tag__, ##__VA_ARGS__));