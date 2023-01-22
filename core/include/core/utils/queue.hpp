#pragma once

/**
 * @file core/utils/queue.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @defgroup Queue
 * @brief Wrapper for thread-safe queue implementations
 * @ingroup Utils
 * @{
 */

#if defined(CONFIG_CORE_QUEUE_STDMUTEX_ENABLE)

#include <queue>
#include <mutex>

namespace core::utils
{
    /**
     * @brief Wrapper class for thread safe std::queue + mutex
     * @ingroup Queue
     * @todo: maybe replace with a lock-free queue? is it worth it? check latency
     */
    template <typename _T>
    class mutex_queue
    {
    private:
        typedef std::queue<_T> _Sequence;

        std::mutex _im; // mutex
        _Sequence  _iq; // internal queue implementation
    protected:
    public:
        typedef typename _Sequence::value_type      value_type;
        typedef typename _Sequence::reference       reference;
        typedef typename _Sequence::const_reference const_reference;
        typedef typename _Sequence::size_type       size_type;
        typedef _Sequence                           container_type;

        mutex_queue()                                = default;
        mutex_queue(const mutex_queue &)             = delete;
        mutex_queue &operator=(const mutex_queue &&) = delete;
        mutex_queue(mutex_queue &&)                  = default;
        mutex_queue &operator=(mutex_queue &)        = default;

        /**
         * @brief Check if the queue is empty.
         * Note that this isn't thread-safe wrapped
         *
         * @return true if the queue is empty
         * @return false if the queue is not elements
         */
        bool was_empty() const { return _iq.empty(); }

        /**
         * @brief Retrieves the count of elements in queue at time of calling.
         * Note that this isn't thread-safe wrapped
         *
         * @return std::size_t count of elements in queue
         */
        std::size_t was_size() const { return _iq.size(); }


        /**
         * @brief Push data to queue. 
         * @todo: make move version?
         *
         * @param o data to be added
         * @return true if the data was added
         * @return false if the data was not added
         */
        bool try_push(value_type &o)
        {
            std::lock_guard<std::mutex> l(_im);
            _iq.push(o);
            return true;
        }

        /**
         * @brief Pop data from queue.
         *
         * @param o where to store the popped data
         * @return true if data was popped
         * @return false if data was not popped
         */
        bool try_pop(value_type &o)
        {
            std::lock_guard<std::mutex> l(_im);
            if (_iq.empty()) return false;
            o = std::move(_iq.front());
            _iq.pop();
            return true;
        }
    };
} // namespace core::utils
#endif

#if defined(CONFIG_CORE_QUEUE_LOCKFREE_ENABLE)

#include <atomic>

namespace core::utils
{

    /**
     * @brief Atomic queue (not tested thoroughly)
     * @ingroup Queue
     * @todo: split cache lines, test more, 
     * 
     * As far as i understand, memory orders / barriers:
     *  1. relaxed -> just ensures that the store/load operations are done atomically
     *  2. consume -> no idea lmao
     *  3. acquire -> a load operation locks all operations (not just read/write in 
     *      current thread, but all operations)
     *  4. release -> a store operation releases releases the previous acquire op 
     *      barrier, thus making all changes visible to all other threads
     *  5. acq_rel -> combination of acquire and release. a read-modify-write operation
     *      is both acquire and release operation. 
     *      in short = read(acquire)-modify-write(release)?
     *  6. seq_cst -> default memory order, a load performs acquire, a load performs
     *      release and read-modify-write are like acq_rel
     * 
     * actualy hw implementation differs, 
     *      in x86, acquire are cheap?
     *      in arm, acquire is expensive?
     * 
     * safest -> to potentially problematic (if you don't know what you are doing)
     *      seq_cst -> relaxed -> acq_rel -> acquire,release,consume
     * slowest -> fastest
     * safest -> slowest and dangerous -> fastest (at your own risk lol)
     * 
     * acquire-release -> ordering is kinda like similar to lock-unlock in mutex?
     *  but it seems to work across the memory? or cache?
     * 
     * Most loads are relaxed so that we don't use RFO operation (since that's slow?)
     *  and we just need to guarantee that the load is atomic.
     *  See MESI_protocol (is this used by most modern processors?)
     * 
     * @tparam _T type of element
     */
    template <typename _T>
    class atomic_queue
    {
    private:
        /// maybe need to set this higher when we fix cache lines
        static constexpr unsigned int _bufsize   = 1024;
        static constexpr bool         __copyable = std::is_copy_assignable<_T>::value;
        static constexpr bool         __moveable = std::is_move_assignable<_T>::value;

        // data states
        enum class _State : unsigned int
        {
            EMPTY = 0,
            LOADING,
            LOADED,
            UNLOADING
        };

        // should we just separate members to separate arrays, instead of struct array?
        struct _node
        {
            _T                  data;
            std::atomic<_State> state;
        };

        static_assert(_bufsize >= 64, "atomic_queue minimum buffer size is 64");
        static_assert(__moveable == true, "atomic_queue requires a move assignable type");

        /** head and tail are converted to array index by doing a modulo */
        _node                     _nodes[_bufsize] = {};
        std::atomic<unsigned int> _head{0};
        std::atomic<unsigned int> _tail{0};
    protected:

    public:
        typedef _T value_type;

        atomic_queue()                                 = default;
        atomic_queue(const atomic_queue &)             = delete;
        atomic_queue &operator=(const atomic_queue &&) = delete;
        atomic_queue(atomic_queue &&)                  = default;
        atomic_queue &operator=(atomic_queue &)        = default;

        /**
         * @brief Checks whether the queue was empty at time of call.
         * Note that this isn't thread-safe wrapped.
         *
         * @return true if the queue is empty
         * @return false if the queue is not empty
         */
        bool was_empty() const noexcept
        {
            // relaxed loads so we don't broadcast RFO
            unsigned int tail = std::atomic_load_explicit(&_tail, std::memory_order_relaxed);
            unsigned int head = std::atomic_load_explicit(&_head, std::memory_order_relaxed);
            return tail == head;
        }

        /**
         * @brief Retrieves the count of elements currently in queue at time of calling.
         * Note that this isn't thread-safe wrapped.
         *
         * @return std::size_t count of elements in queue
         */
        std::size_t was_size() const noexcept
        {
            // relaxed loads so we don't broadcast RFO
            unsigned int tail = std::atomic_load_explicit(&_tail, std::memory_order_relaxed);
            unsigned int head = std::atomic_load_explicit(&_head, std::memory_order_relaxed);
            return tail - head;
        }

        /**
         * @brief Push data to queue
         * @todo: make move version?
         *
         * @param o data to be added
         * @return true if the data was added
         * @return false if the data was not added
         */
        bool try_push(const _T &o) noexcept
        {
            // reserve tail node for pushing new data
            unsigned int tail = std::atomic_load_explicit(&_tail, std::memory_order_relaxed);
            do
            {
                // weak check that we are not behind head
                unsigned int head = std::atomic_load_explicit(&_head, std::memory_order_relaxed);
                if ((tail % _bufsize) - (head % _bufsize) >= _bufsize) return false;
            } while (!std::atomic_compare_exchange_weak_explicit(&_tail,
                                                                 &tail,
                                                                 tail + 1,
                                                                 std::memory_order_acq_rel,
                                                                 std::memory_order_relaxed));

            _node &node     = _nodes[tail % _bufsize];
            _State expected = _State::EMPTY;
            /**
             * we expect state to be empty
             *  if not empty, spin till empty
             * on success, we acquire and set state to LOADING
             * on fail, do relaxed loads
             */
            while (!std::atomic_compare_exchange_weak_explicit(&node.state,
                                                               &expected,
                                                               _State::LOADING,
                                                               std::memory_order_acquire,
                                                               std::memory_order_relaxed))
                expected = _State::EMPTY; // maybe add a spin or yield here?
            // we can now load the data
            node.data = o;
            // do release store to state and set it to LOADED
            std::atomic_store_explicit(&node.state, _State::LOADED, std::memory_order_release);
            return true;
        }

        /**
         * @brief Push data to queue
         * @todo: make move version?
         *
         * @param o data to be added
         * @return true if the data was added
         * @return false if the data was not added
         */
        bool try_push(_T &&o) noexcept
        {
            // reserve tail node for pushing new data
            unsigned int tail = std::atomic_load_explicit(&_tail, std::memory_order_relaxed);
            do
            {
                // weak check that we are not behind head
                unsigned int head = std::atomic_load_explicit(&_head, std::memory_order_relaxed);
                if ((tail % _bufsize) - (head % _bufsize) >= _bufsize) return false;
            } while (!std::atomic_compare_exchange_weak_explicit(&_tail,
                                                                 &tail,
                                                                 tail + 1,
                                                                 std::memory_order_acq_rel,
                                                                 std::memory_order_relaxed));

            _node &node     = _nodes[tail % _bufsize];
            _State expected = _State::EMPTY;
            /**
             * we expect state to be empty
             *  if not empty, spin till empty
             * on success, we acquire and set state to LOADING
             * on fail, do relaxed loads
             */
            while (!std::atomic_compare_exchange_weak_explicit(&node.state,
                                                               &expected,
                                                               _State::LOADING,
                                                               std::memory_order_acquire,
                                                               std::memory_order_relaxed))
                expected = _State::EMPTY; // maybe add a spin or yield here?
            // we can now load the data
            node.data = std::move(o);
            // do release store to state and set it to LOADED
            std::atomic_store_explicit(&node.state, _State::LOADED, std::memory_order_release);
            return true;
        }

        /**
         * @brief Pop data from queue.
         *
         * @param o where to store the popped data
         * @return true if data was popped
         * @return false if data was not popped
         */
        bool try_pop(_T &o) noexcept
        {
            // increment head if there's element inqueue
            unsigned int head = std::atomic_load_explicit(&_head, std::memory_order_relaxed);
            do
            {
                // weak check that there's element inqueue
                unsigned int tail = std::atomic_load_explicit(&_tail, std::memory_order_relaxed);
                if (tail == head) return false;
            } while (!std::atomic_compare_exchange_weak_explicit(&_head,
                                                                 &head,
                                                                 head + 1,
                                                                 std::memory_order_acq_rel,
                                                                 std::memory_order_relaxed));

            _node &node     = _nodes[head % _bufsize];
            _State expected = _State::LOADED;
            /**
             * we expect node to be loaded
             *  if not, we busy-wait till it's loaded
             * on success, we acquire
             * on fail, do relaxed loads
             */
            while (!std::atomic_compare_exchange_weak_explicit(&node.state,
                                                               &expected,
                                                               _State::UNLOADING,
                                                               std::memory_order_acquire,
                                                               std::memory_order_relaxed))
                expected = _State::LOADED; // maybe add a spin or yield here?
            // we get the data
            o = std::move(node.data);
            // set state with release
            std::atomic_store_explicit(&node.state, _State::EMPTY, std::memory_order_release);
            return true;
        }
    };

} // namespace core::utils
#endif

namespace core::utils
{
#if defined(CONFIG_CORE_QUEUE_DEFAULT_STDMUTEX)
    template <typename T>
    using queue = mutex_queue<T>;
#elif defined(CONFIG_CORE_QUEUE_DEFAULT_LOCKFREE)
    template <typename T>
    using queue = atomic_queue<T>;
#else
#error "Missing implementation for core::utils::queue. See CORE_QUEUE_DEFAULT_* for options"
#endif
// #endif
} // namespace core::utils

/** @} endgroup Queue */
