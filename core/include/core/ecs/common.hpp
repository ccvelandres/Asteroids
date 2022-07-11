#pragma once

/**
 * @addtogroup ECS
 * @{
 */

#include <cstdint>
#include <vector>

/**
 * @brief Contains common declaration for the ECS system
 * 
 */

/**
 * @todo Replace component ID with no RTTI
 *
 * template<typename T>
 * struct type { static void id() { } };
 *
 * template<typename T>
 * std::size_t type_id() { return reinterpret_cast<std::size_t>(&type<T>::id); }
 */

using ComponentID                   = std::size_t;
constexpr std::size_t maxComponents = 32;

inline ComponentID getComponentID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentID() noexcept
{
    static ComponentID cID = getComponentID();
    return cID;
}

/** @} endgroup ECS */