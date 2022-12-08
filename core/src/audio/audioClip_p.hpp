#pragma once

/**
 * @file core/audio/audioClip_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the audioClip class
 * @addtogroup Audio
 * @{
 */

#include <audio/audioManager_p.hpp>

struct AudioClip::Internal
{
    /** Audio clip object that owns this internal data */
    const AudioClip* clip;
    /** Shared pointer to audio data */
    std::shared_ptr<core::audio::manager::AudioData> audioData;
};

/** @} endgroup Audio */
