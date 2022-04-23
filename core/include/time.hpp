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
    
    time_ds m_unscaledFrameDelta;
    time_ds m_unscaledFrameTime;
    time_ds m_unscaledFrameStart;
    time_ds m_unscaledFrameEnd;

    float m_timeScale;
    time_ds m_scaledFrameDelta;
    time_ds m_scaledFrameTime;
    time_ds m_scaledFrameStart;
    time_ds m_scaledFrameEnd;
    time_ds m_scaledTime;

protected:
public:
    Time() : m_timeScale(1) {}

    void preUpdate()
    {
        using namespace std::chrono;
        /** unscaled update */
        m_unscaledFrameStart = getTime<time_ds>();
        m_unscaledFrameDelta = m_unscaledFrameStart - m_unscaledFrameEnd;

        /** scaled update */
        m_scaledFrameDelta = m_unscaledFrameDelta * m_timeScale;
        m_scaledTime = m_scaledFrameStart += m_scaledFrameDelta;
    }

    void postUpdate()
    {
        /** scaled update */
        m_scaledFrameEnd = getTime<time_ds>();
        m_scaledFrameTime = m_scaledFrameEnd - m_scaledFrameStart;

        /** unscaled update */
        m_unscaledFrameTime = m_unscaledFrameTime * m_timeScale;
        m_unscaledFrameEnd = m_unscaledFrameStart + m_unscaledFrameTime;
    }

    void timeScale(const float& scale) { m_timeScale = scale; }
    float timeScale() { return m_timeScale; }

    time_ds scaledDeltaTime() { return m_scaledFrameDelta; }
    time_ds scaledFrameTime() { return m_scaledFrameTime; }
    time_ds scaledTime() { return m_scaledTime; }

    time_ds unscaledDeltaTime() { return m_unscaledFrameDelta; }
    time_ds unscaledFrameTime() { return m_unscaledFrameTime; }
    time_ds unscaledTime() { return getTime<time_ds>(); }

    /**  */
    template <typename T>
    static T getTime()
    {
        return std::chrono::duration_cast<T>(clock::now() - m_startTime);
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