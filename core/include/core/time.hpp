#pragma once

#include <chrono>

using time_type = int64_t;
using time_step = std::micro;
using time_ds   = std::chrono::duration<time_type, time_step>;
using time_fs   = std::chrono::duration<float>;
using time_ms   = std::chrono::milliseconds;
using time_us   = std::chrono::microseconds;
using time_ns   = std::chrono::nanoseconds;

class Time
{
public:
    /** Clock used by Time class */
    typedef std::chrono::high_resolution_clock clock;

private:
    static clock::time_point m_startTime;

    time_ds m_unscaledFrameDelta;
    time_ds m_unscaledFrameTime;
    time_ds m_unscaledFrameStart;
    time_ds m_unscaledFrameEnd;
    time_ds m_unscaledTime;

    float   m_timeScale;
    time_ds m_scaledFrameDelta;
    time_ds m_scaledFrameTime;
    time_ds m_scaledFrameStart;
    time_ds m_scaledFrameEnd;
    time_ds m_scaledTime;

protected:
public:
    Time();
    /** Unless needing the exact time, use unscaledTime as much as possible */
    template <typename T>
    static T getTime()
    {
        return std::chrono::duration_cast<T>(clock::now() - m_startTime);
    }

    /** Called every start of game loop update */
    void preUpdate();
    /** Called before ending game loop update */
    void postUpdate();

    /** Set the time scale for scaled time */
    void timeScale(const float &scale) { m_timeScale = scale; }
    /** Retrieve time scale for scaled time */
    float timeScale() const { return m_timeScale; }

    /** Get scaled delta frame time */
    template <typename T = time_ds>
    T scaledDeltaTime() const
    {
        return std::chrono::duration_cast<T>(m_scaledFrameDelta);
    }
    /** Get the scaled frame time */
    template <typename T = time_ds>
    T scaledFrameTime() const
    {
        return std::chrono::duration_cast<T>(m_scaledFrameTime);
    }
    /** Get elapsed scaled time */
    template <typename T = time_ds>
    T scaledTime() const
    {
        return std::chrono::duration_cast<T>(m_scaledTime);
    }

    /** Get unscaled delta frame time */
    template <typename T = time_ds>
    T unscaledDeltaTime() const
    {
        return std::chrono::duration_cast<T>(m_unscaledFrameDelta);
    }
    /** Get unscaled frame start time */
    template <typename T = time_ds>
    T unscaledFrameStart() const
    {
        return std::chrono::duration_cast<T>(m_unscaledFrameStart);
    }
    /** Get unscaled frame end time */
    template <typename T = time_ds>
    T unscaledFrameEnd() const
    {
        return std::chrono::duration_cast<T>(m_unscaledFrameEnd);
    }
    /** Get unscaled frame time */
    template <typename T = time_ds>
    T unscaledFrameTime() const
    {
        return std::chrono::duration_cast<T>(m_unscaledFrameTime);
    }
    /** Get elapsed unscaled time */
    template <typename T = time_ds>
    T unscaledTime()
    {
        return std::chrono::duration_cast<T>(m_unscaledTime);
    }

    static constexpr double ticksPerSecond =
        std::chrono::duration<double, clock::period>(std::chrono::seconds(1)).count();

    /** Get elapsed time in milliseconds */
    time_ms millis() { return std::chrono::duration_cast<time_ms>(m_unscaledTime); }
    /** Get elapsed time in microseconds */
    time_us micros() { return std::chrono::duration_cast<time_us>(m_unscaledTime); }
    /** Get elapsed time in nanoseconds */
    time_ns nano() { return std::chrono::duration_cast<time_ns>(m_unscaledTime); }
};