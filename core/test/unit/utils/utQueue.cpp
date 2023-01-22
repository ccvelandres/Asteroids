#include <gtest/gtest.h>
#include <gmock/gmock.h>
// #include <gtest/gtest-matchers.h>
#include <generated/config.h>
#include <core/utils/queue.hpp>

TEST(AtomicQueueTest, PushTestInt)
{
    core::utils::atomic_queue<int> aQueue;

    // Push local and rvalue
    int pushVal = 0;
    aQueue.try_push(pushVal);
    aQueue.try_push(1);

    int popVal;
    ASSERT_EQ(aQueue.try_pop(popVal), true);
    ASSERT_EQ(popVal, 0);
    ASSERT_EQ(aQueue.try_pop(popVal), true);
    ASSERT_EQ(popVal, 1);
    ASSERT_EQ(aQueue.try_pop(popVal), false);
}

TEST(AtomicQueueTest, PushTestStruct)
{
    struct payloadStruct
    {
        int a;
        int b;

        payloadStruct() : a(0), b(0) {}
        payloadStruct(int _a, int _b) : a(_a), b(_b) {}
    };

    core::utils::atomic_queue<payloadStruct> aQueue;

    // Push local and rvalue
    payloadStruct a = payloadStruct(1, 2);
    aQueue.try_push(a);
    aQueue.try_push(payloadStruct(3, 4));

    payloadStruct popVal;
    ASSERT_EQ(aQueue.try_pop(popVal), true);
    ASSERT_EQ(popVal.a, 1);
    ASSERT_EQ(popVal.b, 2);
    ASSERT_EQ(aQueue.try_pop(popVal), true);
    ASSERT_EQ(popVal.a, 3);
    ASSERT_EQ(popVal.b, 4);
    ASSERT_EQ(aQueue.try_pop(popVal), false);
}

TEST(AtomicQueueTest, PushTestStructRawPtr)
{
    struct payloadStruct
    {
        int a;
        int b;

        payloadStruct() : a(0), b(0) {}
        payloadStruct(int _a, int _b) : a(_a), b(_b) {}
    };

    core::utils::atomic_queue<payloadStruct *> aQueue;

    // Push local and rvalue
    payloadStruct a = payloadStruct(1, 2);
    aQueue.try_push(&a);
    aQueue.try_push(NULL);

    payloadStruct *popVal;
    ASSERT_EQ(aQueue.try_pop(popVal), true);
    ASSERT_EQ(popVal->a, 1);
    ASSERT_EQ(popVal->b, 2);
    ASSERT_EQ(aQueue.try_pop(popVal), true);
    ASSERT_THAT(popVal, ::testing::IsNull());
    ASSERT_EQ(aQueue.try_pop(popVal), false);
}

TEST(AtomicQueueTest, PushTestCopyableMoveable)
{
    struct payloadStruct
    {
        payloadStruct()                           = default;
        payloadStruct(const payloadStruct &)       = default;
        payloadStruct &operator=(payloadStruct &)  = default;
        payloadStruct(payloadStruct &&)      = default;
        payloadStruct &operator=(payloadStruct &&) = default;
    };

    core::utils::atomic_queue<payloadStruct> aQueue;
}