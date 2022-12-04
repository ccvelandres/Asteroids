#pragma once

/**
 * @file core/utils/memory.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @defgroup Memory
 * @brief Wrapper for C++ memory types
 * @ingroup Utils
 * @{
 */

#include "logging.hpp"
#include <memory>
#include <vector>
#include <algorithm>

namespace core::utils::memory
{

    template<typename T>
    using weak_ptr = std::weak_ptr<T>;

    template<typename T>
    using shared_ptr = std::shared_ptr<T>;

    template<typename T>
    using unique_ptr = std::unique_ptr<T>;

    /**
     * @brief Convert a vector of weak_ptr to shared_ptr
     * 
     * @tparam T ptr type
     * @param v vector to convert
     * @return std::vector<std::shared_ptr<T>>&& 
     */
    template<typename T>
    static inline std::vector<shared_ptr<T>> &&convert_ptr(std::vector<weak_ptr<T>> &v)
    {
        std::vector<shared_ptr<T>> v_out;
        std::transform(v.begin(), v.end(), v_out.begin(),
            [](std::weak_ptr<T> &c) { return c.lock(); });
        return std::move(v_out);
    }

} // namespace core::utils::memory

/** @} endgroup Memory */