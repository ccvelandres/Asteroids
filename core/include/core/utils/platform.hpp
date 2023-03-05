#pragma once

/**
 * @file core/utils/platform.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @defgroup Macro
 * @brief Wrapper for platform dependent function/macro
 * @ingroup Utils
 * @{
 */

#include "logging.hpp"
#include <filesystem>

namespace core::utils::platform
{

#ifdef __linux__

#elif _WIN32
#include <libloaderapi.h>
#include <errhandlingapi.h>
#else
#error "Could not detect platform"
#endif

    /**
     * @brief Return a filesystem path object containing the path to the executable
     *
     * @return std::filesystem::path path to executable
     */
    inline std::filesystem::path getProjectPath()
    {
#ifdef __linux__
        return std::filesystem::canonical("/proc/self/exe").parent_path();
#elif _WIN32
        L_TAG("getProjectPath");
        constexpr int max_path_length = 256;
        TCHAR         szPath[max_path_length];
        if (!GetModuleFileName(NULL, szPath, max_path_length))
        {
            L_THROW(std::runtime_error, "Could not retrieve module path (%d)" , GetLastError());
        }

        return std::filesystem::path(szPath).parent_path();
#else
#error "Could not detect platform"
#endif
    }
} // namespace core::utils::macro

/** @} endgroup JSON */