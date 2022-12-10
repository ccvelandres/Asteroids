#pragma once

/**
 * @file core/audio/audioManager_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the audioManager
 * @addtogroup Audio
 * @{
 */

#include <core/audio/audioManager.hpp>
#include <core/ecs/components/audioComponent.hpp>

#include <SDL2/SDL_audio.h>

namespace core::audio
{
    struct AudioData
    {
        uint32_t                 audioLength;
        std::unique_ptr<uint8_t> audioBuffer;
        SDL_AudioSpec            audioSpec;
    };

};

/** @} endgroup Audio */
