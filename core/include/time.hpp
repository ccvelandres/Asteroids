#pragma once

#include <chrono>

using time_s = float;
using time_ds = std::chrono::duration<time_s>;
using time_ms = std::chrono::milliseconds;
using time_us = std::chrono::microseconds;
using time_ns = std::chrono::nanoseconds;

class Time
{
public:
    typedef std::chrono::high_resolution_clock clock;
    typedef std::nano ticks;
    typedef std::chrono::nanoseconds period;

private:
    const static clock::time_point m_startTime;

protected:
public:
    Time() {}

    template <typename T>
    static T getTime()
    {
        using namespace std::chrono;
        auto t = clock::now();
        return duration_cast<T>(t - m_startTime);
    }

    static constexpr double ticksPerSecond = std::chrono::duration<double, ticks>(std::chrono::seconds(1)).count();

    static time_ds time()
    {
        return duration_cast<time_ds>(clock::now() - m_startTime);
    }

    static time_ms millis() { return getTime<time_ms>(); }
    static time_us micros() { return getTime<time_us>(); }
    static time_ns nano() { return getTime<time_ns>(); }
};