#include <gtest/gtest.h>
#include <thread>
#include "ThreadSafeQueue.h"
#include "Order.h"

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_queuePush_Test)
{
    obe::ThreadSafeQueue<int> q;
    for (int i = 1 ; i < 501 ; ++i)
    {
        q.push(i);
    }

    EXPECT_EQ(q.size(), 500);
    auto x = q.pop();
    EXPECT_TRUE(x.has_value());
    EXPECT_EQ(x, 1);
    EXPECT_FALSE(q.empty());
}

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_queuePop_Test)
{
    obe::ThreadSafeQueue<int> q;
    for (int i = 1 ; i < 501 ; ++i)
    {
        q.push(i);
    }

    EXPECT_EQ(q.size(), 500);
    EXPECT_EQ(q.pop().value(), 1);
    EXPECT_EQ(q.size(), 499);
    EXPECT_FALSE(q.empty());
}

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_queuePopEmptyQueue_Test)
{
    obe::ThreadSafeQueue<int> q;
    std::optional<int> i{};
    std::jthread thread([&](){
        i = q.pop();

    });

    q.stop();

    EXPECT_EQ(q.size(), 0);
    EXPECT_FALSE(i.has_value());
    EXPECT_TRUE(q.empty()); 
}


TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_queuePopTwo_Test)
{
    obe::ThreadSafeQueue<int> q;
    q.push(1);
    q.push(2);

    EXPECT_EQ(q.size(), 2);
    EXPECT_EQ(q.pop().value(), 1);
    EXPECT_EQ(q.size(), 1);
    EXPECT_EQ(q.pop().value(), 2);
    EXPECT_TRUE(q.empty());
}

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_twoThreadsPushPop_Test)
{
    obe::ThreadSafeQueue<int> q;
    std::jthread threadPop([&](){
        auto x = q.pop();

        if (x.has_value())
        {
           EXPECT_EQ(x.value(),1);
        }

        x = q.pop();

        if (x.has_value())
        {
           EXPECT_EQ(x.value(),2);
        }
        x = q.pop();

        if (x.has_value())
        {
           EXPECT_EQ(x.value(),3);
        }

        x = q.pop();

        if (x.has_value())
        {
           EXPECT_EQ(x.value(),4);
        }
        x = q.pop();

        if (x.has_value())
        {
           EXPECT_EQ(x.value(),5);
        }

    });

    std::jthread threadPush([&](){
        q.push(1);
        q.push(2);
        q.push(3);
        q.push(4);
        q.push(5);
    });

    q.stop();
}

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_tryPopEmpty_Test)
{
    obe::ThreadSafeQueue<int> q;
    int x{0};
    EXPECT_FALSE(q.tryPop(&x));
}

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_tryPop_Test)
{
    obe::ThreadSafeQueue<int> q;
    q.push(1);
    int x{0};
    EXPECT_TRUE(q.tryPop(&x));
    EXPECT_EQ(x, 1);
}