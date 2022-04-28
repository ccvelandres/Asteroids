#include <time.hpp>

using namespace std::chrono;

const high_resolution_clock::time_point Time::m_startTime = high_resolution_clock::now();

void Time::preUpdate()
{
    using namespace std::chrono;
    /** unscaled update */
    m_unscaledTime = m_unscaledFrameStart = getTime<time_ds>();
    m_unscaledFrameDelta = m_unscaledFrameStart - m_unscaledFrameEnd;

    /** scaled update */
    m_scaledFrameDelta = duration_cast<time_ds>(m_unscaledFrameDelta * m_timeScale);
    m_scaledTime = m_scaledFrameStart += m_scaledFrameDelta;
}

void Time::postUpdate()
{
    /** scaled update */
    m_unscaledFrameEnd = getTime<time_ds>();
    m_unscaledFrameTime = m_unscaledFrameEnd - m_unscaledFrameStart;

    /** unscaled update */
    m_scaledFrameTime = duration_cast<time_ds>(m_unscaledFrameTime * m_timeScale);
    m_scaledFrameEnd = m_scaledFrameStart + m_scaledFrameTime;
}