#pragma once

/**
 * @file core/utils/hash.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @defgroup Hash
 * @brief Interface file for hash library.
 * @ingroup Utils
 * @{
 */

#include <algorithm>

namespace core::utils
{
    inline void hash_combine(std::size_t &seed) {}

    template <typename T, typename... HashValues>
    inline void hash_combine(std::size_t &seed, const T &v, HashValues... hashValues)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        hash_combine(seed, hashValues...);
    }
} // namespace core::utils

/** @} endgroup JSON */