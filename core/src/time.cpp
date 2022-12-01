#include <time.hpp>
#include <utils/logging.hpp>

using namespace std::chrono;

high_resolution_clock::time_point Time::m_startTime = high_resolution_clock::now();

Time::Time() : m_timeScale(1)
{
    L_TAG("Time::Time");

    L_DEBUG("startTime {}", m_startTime.time_since_epoch().count());

    this->m_unscaledFrameDelta = time_ds(0);
    this->m_unscaledFrameTime  = time_ds(0);
    this->m_unscaledFrameStart = time_ds(0);
    this->m_unscaledFrameEnd   = time_ds(0);
    this->m_unscaledTime       = time_ds(0);
    this->m_scaledFrameDelta   = time_ds(0);
    this->m_scaledFrameTime    = time_ds(0);
    this->m_scaledFrameStart   = time_ds(0);
    this->m_scaledFrameEnd     = time_ds(0);
    this->m_scaledTime         = time_ds(0);
}

void Time::preUpdate()
{
    using namespace std::chrono;
    /** unscaled update */
    m_unscaledTime = m_unscaledFrameStart = getTime<time_ds>();
    m_unscaledFrameDelta                  = m_unscaledFrameStart - m_unscaledFrameEnd;

    /** scaled update */
    m_scaledFrameDelta = duration_cast<time_ds>(m_unscaledFrameDelta * m_timeScale);
    m_scaledTime       = m_scaledFrameStart += m_scaledFrameDelta;
}

void Time::postUpdate()
{
    /** scaled update */
    m_unscaledFrameEnd  = getTime<time_ds>();
    m_unscaledFrameTime = m_unscaledFrameEnd - m_unscaledFrameStart;

    /** unscaled update */
    m_scaledFrameTime = duration_cast<time_ds>(m_unscaledFrameTime * m_timeScale);
    m_scaledFrameEnd  = m_scaledFrameStart + m_scaledFrameTime;
}