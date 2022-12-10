#pragma once

/**
 * @file core/manager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Core
 * @{
 */

#include "time.hpp"

namespace core
{
    /**
     * @brief All Managers are singleton
     * @todo: is CRTP a good idea for managers? 
     * @tparam T manager type
     */
    template<typename T>
    class Manager
    {
    private:
    protected:
        Manager() {}
    public:

        ~Manager() {}
        Manager(Manager &o)             = delete;
        Manager(Manager &&o)            = delete;
        Manager &operator=(Manager &o)  = delete;
        Manager &operator=(Manager &&o) = delete;

        /**
         * @brief Retrieve the manager instance
         * 
         * @return T& reference to the manager instance
         */
        static T& Instance() {
            static T instance = T();
            return instance;
        };

        /**
         * @brief Initialize the manager
         *
         * @return true on success
         * @return false on failure
         */
        bool init()
        {
            return static_cast<T&>(*this).init();
        }

        /**
         * @brief Preupdate step for manager
         *
         */
        void preUpdate()
        {
            static_cast<T&>(*this).preUpdate();
        }

        /**
         * @brief Fixed interval update for manager
         *
         * @param delta time in ms
         */
        void fixedUpdate(const time_ms &delta)
        {
            static_cast<T&>(*this).fixedUpdate(delta);
        }

        /**
         * @brief Scaled time update for manager
         *
         * @param delta time in ms
         */
        void update(const time_ms &delta)
        {
            static_cast<T&>(*this).update(delta);
        }

        /**
         * @brief Post update step for manager
         *
         */
        void postUpdate()
        {
            static_cast<T&>(*this).postUpdate();
        }

        /**
         * @brief Refresh step for manager. Cleans unused resources
         *
         */
        void refresh()
        {
            static_cast<T&>(*this).refresh();
        }
    };
}; // namespace core

/** @} endgroup Core */
