#pragma once

#include <spdlog/spdlog.h>

/** We use alias for spdlog in case we change it in the future */
namespace logging {
    using namespace spdlog;
}

/** Helper macros for appending the line and function */
#define L_TAG(STR) static const std::string tag__{STR}

#define L_LOG(level, STR, ...) logging::level("[{}] " STR, tag__, ##__VA_ARGS__)
#define L_ERROR(STR, ...) L_LOG(error, STR, ##__VA_ARGS__)
#define L_WARNING(STR, ...) L_LOG(warning, STR, ##__VA_ARGS__)
#define L_INFO(STR, ...) L_LOG(info, STR, ##__VA_ARGS__)
#define L_DEBUG(STR, ...) L_LOG(debug, STR, ##__VA_ARGS__)
#define L_TRACE(STR, ...) L_LOG(trace, STR, ##__VA_ARGS__)
