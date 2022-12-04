#pragma once

/**
 * @file core/audio/audioClip_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the audioClip class
 * @addtogroup Audio
 * @{
 */

#include <core/audio/audioManager.hpp>
#include <core/ecs/components/audioComponent.hpp>

#include <SDL2/SDL_audio.h>

struct AudioClip::Internal
{
    uint32_t                 audioLength;
    std::unique_ptr<uint8_t> audioBuffer;
    SDL_AudioSpec            audioSpec;
};

/** @} endgroup Audio */
