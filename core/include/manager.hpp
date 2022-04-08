#pragma once

#include <type_traits>

/**
 * @brief Interface class for Manager objects (CRTP)
 * @todo static asserts to check that methods are implemented
 */
template <class M>
class Manager
{
public:
    Manager()
    {
        static_assert((void*)(&M::preUpdate) == (void*)(&Manager::preUpdate),
            "Derid didnt implement");
        // static_assert(,
        //               "Derived class must define preUpdate function");
    }
    void init() { static_cast<M *>(this)->init(); }
    void preUpdate()
    {
        static_cast<M *>(this)->preUpdate();
    }
    void postUpdate() { static_cast<M *>(this)->postUpdate(); }
    void update() { static_cast<M *>(this)->update(); }
    void refresh() { static_cast<M *>(this)->refresh(); }
    void clean() { static_cast<M *>(this)->clean(); }
};