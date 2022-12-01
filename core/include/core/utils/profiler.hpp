#pragma once

/** Interface file for easy_profiler */

#if defined(CONFIG_CORE_PROFILER_ENABLE)
#include <easy/arbitrary_value.h>
#include <easy/profiler.h>

/** Profiler Macros */
#define PROFILER_START(...) \
    EASY_PROFILER_ENABLE;   \
    profiler::startListen();
#define PROFILER_BLOCK(...) EASY_BLOCK(__VA_ARGS__)
#define PROFILER_VALUE(...) EASY_VALUE(__VA_ARGS__)
#define PROFILER_VIN(...)   EASY_VIN(__VA_ARGS__)
#define PROFILER_END(...)   profiler::dumpBlocksToFile("dump.prof")

#else

/** Profiler Macros */
#define PROFILER_START(...)
#define PROFILER_BLOCK(...)
#define PROFILER_VALUE(...)
#define PROFILER_VIN(...)
#define PROFILER_END(...)
#endif