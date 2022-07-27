
#include <core/utils/logging.hpp>
#include <core/utils/queue.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <cassert>
#include <queue>
#include <thread>
#include <chrono>
#include <iostream>
#include <condition_variable>

template <typename Queue>
struct QueueWrapper : private Queue
{
    typedef typename Queue::value_type value_type;

    bool empty() noexcept
    {
        return static_cast<Queue &>(*this).was_empty();
    }

    std::size_t size() noexcept
    {
        return static_cast<Queue &>(*this).was_size();
    }

    bool push(value_type &o)
    {
        return static_cast<Queue &>(*this).try_push(o);
    }

    bool pop(value_type &o)
    {
        return static_cast<Queue &>(*this).try_pop(o);
    }
};

struct Signal
{
    std::mutex              mutex;
    std::condition_variable cond;
    std::atomic<bool>       startFlag{false};
    std::atomic<bool>       stopFlag{false};

    void wait()
    {
        if (startFlag.load() == true) return;
        std::unique_lock<std::mutex> l(mutex);
        cond.wait(l);
        l.unlock();
    }

    void start()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        startFlag.store(true, std::memory_order_acq_rel);
        cond.notify_all();
    }

    void stop() { stopFlag.store(true, std::memory_order_acq_rel); }

    Signal() = default;
    Signal(bool _start) : startFlag(_start), stopFlag(false) {}
};

typedef int64_t                                                time_point;
typedef unsigned long long                                     sum_t;
std::chrono::high_resolution_clock::time_point                 programStart = std::chrono::high_resolution_clock::now();
typedef std::chrono::high_resolution_clock::time_point::period time_point_period;
time_point                                                     getTime()
{
    auto t = std::chrono::high_resolution_clock::now() - programStart;
    return t.count();
}

template <class Queue>
void producer_impl(Queue                   *queue,
                   Signal                  *signal,
                   unsigned int             count,
                   std::atomic<int>        *pid,
                   time_point              *pstart,
                   time_point              *pend,
                   std::atomic<time_point> *fstart)
{
    using T = typename Queue::value_type;
    signal->wait();
    *pstart = getTime();

    // first producer saves start time
    time_point expected = 0;
    std::atomic_compare_exchange_strong_explicit(fstart,
                                                 &expected,
                                                 getTime(),
                                                 std::memory_order_acq_rel,
                                                 std::memory_order_relaxed);

    for (unsigned int i = 0; i < count; i++)
    {
        while(!queue->push(i));
    }

    *pend = getTime();
    // last producer
    if (std::atomic_fetch_sub_explicit(pid, 1, std::memory_order_acq_rel) == 1) signal->stop();
}

template <class Queue>
void consumer_impl(Queue            *queue,
                   Signal           *signal,
                   sum_t            *csum,
                   std::atomic<int> *cid,
                   time_point       *cstart,
                   time_point       *cend,
                   time_point       *fend)
{
    using T = typename Queue::value_type;

    signal->wait();
    *cstart = getTime();

    unsigned int elementCount = 0;
    sum_t        sum          = 0;
    for (;;)
    {
        bool stop = signal->stopFlag.load(std::memory_order_relaxed);
        if (stop && queue->empty()) break;
        T n;
        if (queue->pop(n))
        {
            sum += n;
            elementCount++;
        }
    }
    *cend = getTime();

    assert(elementCount != 0);

    // last consumer
    if (std::atomic_fetch_sub_explicit(cid, 1, std::memory_order_acq_rel) == 1) *fend = (getTime());
    *csum = sum;
}

template <class Queue>
void latency_impl(Queue *in, Queue *out, Signal *signal)
{
    using T = typename Queue::value_type;
    for (;;)
    {
        T t_in;
        if (in->pop(t_in))
        {
            out->push(t_in);
        }
        if (signal->stopFlag.load(std::memory_order_relaxed)) break;
    }
}

template <class Queue>
void runBenchmark(const std::string &name, int producerCount, int consumerCount, bool readable = true)
{

    constexpr int       runs = 3;
    std::vector<double> producerTimeRuns(runs);
    std::vector<double> consumerTimeRuns(runs);
    std::vector<double> producerThroughputRuns(runs);
    std::vector<double> consumerThroughputRuns(runs);
    std::vector<double> totalTimeRuns(runs);

    for (int run = 0; run < runs; run++)
    {
        Queue  queue;
        Signal signal;

        constexpr int           elementCount = 1000000;
        std::atomic<int>        producerID{producerCount};
        std::vector<time_point> producerStart(producerCount);
        std::vector<time_point> producerEnd(producerCount);

        std::vector<sum_t>      consumerSums(consumerCount);
        std::atomic<int>        consumerID{consumerCount};
        std::vector<time_point> consumerStart(consumerCount);
        std::vector<time_point> consumerEnd(consumerCount);

        std::atomic<time_point> firstStart{0};
        time_point              lastEnd{0};

        std::vector<std::thread> threads(producerCount + consumerCount - 1);

        for (unsigned int i = 0; i < producerCount; i++)
        {
            threads[i] = std::thread(producer_impl<Queue>,
                                     &queue,
                                     &signal,
                                     elementCount,
                                     &producerID,
                                     &producerStart[i],
                                     &producerEnd[i],
                                     &firstStart);
        }
        for (unsigned int i = 0; i < consumerCount - 1; i++)
        {
            threads[i + producerCount] = std::thread(consumer_impl<Queue>,
                                                     &queue,
                                                     &signal,
                                                     &consumerSums[i],
                                                     &consumerID,
                                                     &consumerStart[i],
                                                     &consumerEnd[i],
                                                     &lastEnd);
        }

        signal.start();
        consumer_impl<Queue>(&queue,
                             &signal,
                             &consumerSums[consumerCount - 1],
                             &consumerID,
                             &consumerStart[consumerCount - 1],
                             &consumerEnd[consumerCount - 1],
                             &lastEnd);

        for (auto &t : threads)
            t.join();

        time_point averageProducerTime = 0;
        for (int i = 0; i < producerCount; i++)
            averageProducerTime += producerEnd[i] - producerStart[i];
        producerTimeRuns[run] = (static_cast<double>(averageProducerTime) / producerCount) / time_point_period::den;

        time_point averageConsumerTime = 0;
        for (int i = 0; i < consumerCount; i++)
            averageConsumerTime += consumerEnd[i] - consumerStart[i];
        consumerTimeRuns[run] = (static_cast<double>(averageConsumerTime) / consumerCount) / time_point_period::den;

        totalTimeRuns[run]         = static_cast<double>(lastEnd - firstStart.load()) / time_point_period::den;
        unsigned int totalMessages = producerCount * elementCount;

        producerThroughputRuns[run] = (static_cast<double>(totalMessages) / static_cast<double>(producerTimeRuns[run]));
        consumerThroughputRuns[run] = (static_cast<double>(totalMessages) / static_cast<double>(consumerTimeRuns[run]));
    }

    double averageProducerTime       = 0.0f;
    double averageProducerThroughput = 0.0f;

    double averageConsumerTime       = 0.0f;
    double averageConsumerThroughput = 0.0f;

    for (auto t : producerTimeRuns)
        averageProducerTime += t;
    averageProducerTime /= producerTimeRuns.size();
    for (auto t : producerThroughputRuns)
        averageProducerThroughput += t;
    averageProducerThroughput /= producerThroughputRuns.size();

    for (auto t : consumerTimeRuns)
        averageConsumerTime += t;
    averageConsumerTime /= consumerTimeRuns.size();
    for (auto t : consumerThroughputRuns)
        averageConsumerThroughput += t;
    averageConsumerThroughput /= consumerThroughputRuns.size();

    if (readable)
    {
        std::printf("Benchmark done: %s\n", name.c_str());
        std::printf("   Producers: %3d\tTotalTime: %8f\tThroughput: %8f\n",
                    producerCount,
                    averageProducerTime,
                    averageProducerThroughput);
        std::printf("   Consumers: %3d\tTotalTime: %8f\tThroughput: %8f\n",
                    consumerCount,
                    averageConsumerTime,
                    averageConsumerThroughput);
    }
    else
    {
        std::printf("%s, %d, %d, %f, %f, %f, %f\n",
                    name.c_str(),
                    producerCount,
                    consumerCount,
                    averageProducerTime,
                    averageProducerThroughput,
                    averageConsumerTime,
                    averageConsumerThroughput);
    }
}

template <class Queue>
void runLatencyTest(const std::string &name, bool readable = true)
{
    using T = typename Queue::value_type;

    constexpr int           runs = 3;
    std::vector<time_point> latencyRuns(runs);

    for (int i = 0; i < runs; i++)
    {
        Queue       in, out;
        Signal      signal(true);
        std::thread loopThread = std::thread(latency_impl<Queue>, &in, &out, &signal);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        time_point start = getTime();
        T          t_in  = i;
        in.push(t_in);
        T t_out;
        while (!out.pop(t_out))
        {
        }
        assert(t_in == t_out);
        time_point end = getTime();

        latencyRuns[i] = end - start;

        signal.stop();
        loopThread.join();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    double averageLatency = 0.0f;
    for (auto t : latencyRuns)
        averageLatency += t;
    averageLatency /= latencyRuns.size();

    if (readable)
    {
        std::printf("Latency Test done: %s\n", name.c_str());
        for (int i = 0; i < latencyRuns.size(); i++)
            std::printf("   Run %d: %ld ns\n", i, latencyRuns[i]);
        std::printf("   Average: %8f ns\n", averageLatency);
    }
    else
    {
        std::printf("%s, %f", name.c_str(), averageLatency);
        for (auto t : latencyRuns)
            std::printf(",%ld", t);
        std::printf("\n");
    }
}

template <class Queue>
void benchmark(const std::string &name, bool readable = true)
{
    const std::pair<std::vector<int>, std::vector<int>> threadRatio = {
        {1,  2, 3, 4, 5, 6, 7, 8, 9, 10, 12},
        {1, 2,      3,  4  }
    };

    // do benchmark for each combination
    for (auto producerCount : threadRatio.first)
    {
        for (auto consumerCount : threadRatio.second)
        {
            runBenchmark<Queue>(name, producerCount, consumerCount, readable);
        }
    }
}

int main()
{
    L_TAG("main");
    bool readable = false;

    typedef QueueWrapper<core::utils::atomic_queue<unsigned int>> AtomicQueueWrapper;
    typedef QueueWrapper<core::utils::mutex_queue<unsigned int>> MutexQueueWrapper;

    std::printf("%s, %s, %s, %s, %s, %s, %s\n",
                "name",
                "producers",
                "consumers",
                "averageProducerTime",
                "averageProducerThroughput",
                "averageConsumerTime",
                "averageConsumerThroughput");
    benchmark<AtomicQueueWrapper>("atomic_queue", readable);
    benchmark<MutexQueueWrapper>("mutex_queue", readable);

    std::printf("%s, %s, %s, %s\n", "name", "averageLatency", "runs", "...");
    runLatencyTest<AtomicQueueWrapper>("atomic_queue", readable);
    runLatencyTest<MutexQueueWrapper>("mutex_queue", readable);

    return 0;
}