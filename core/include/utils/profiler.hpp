#pragma once

/** Interface file for easy_profiler */
// #define PROFILER_ENABLE

#ifdef PROFILER_ENABLE
#include <easy/profiler.h>
#include <easy/arbitrary_value.h>
#else 
#define EASY_BLOCK(...)
#define EASY_VALUE(...)
#define EASY_PROFILER_ENABLE
namespace profiler {
    static void startListen() {}
    static void dumpBlocksToFile(const char* c) {}
}
#endif