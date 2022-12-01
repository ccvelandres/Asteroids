#pragma once

/**
 * @file core/utils/json.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @defgroup JSON
 * @brief Interface file for json library. For sources using json impl, use
 *  utils/json_impl.hpp
 * @ingroup Utils
 * @{
 */

#include <nlohmann/json_fwd.hpp>

namespace core::utils
{
    using json = nlohmann::json;
} // namespace core::utils

/** @} endgroup JSON */