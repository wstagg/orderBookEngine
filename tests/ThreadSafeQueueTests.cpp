#include <gtest/gtest.h>
#include <thread>
#include "ThreadSafeQueue.h"
#include "Order.h"

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_queuePush_Test)
{
    obe::ThreadSafeQueue<int> q;
    for (int i = 0 ; i < 500 ; ++i)
    {
        q.push(i);
    }

    EXPECT_EQ(q.size(), 500);
    EXPECT_EQ(q.pop(), 499);
    EXPECT_FALSE(q.empty());
}

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_queuePop_Test)
{
    obe::ThreadSafeQueue<int> q;
    for (int i = 0 ; i < 500 ; ++i)
    {
        q.push(i);
    }

    EXPECT_EQ(q.size(), 500);
    EXPECT_EQ(q.pop(), 499);
    EXPECT_EQ(q.size(), 499);
    EXPECT_FALSE(q.empty());

}

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_queuePopEmptyQueue_Test)
{
    obe::ThreadSafeQueue<int> q;

    EXPECT_EQ(q.size(), 0);
    EXPECT_TRUE(q.empty()); 
}


TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_queuePopTwo_Test)
{
    obe::ThreadSafeQueue<int> q;
    q.push(1);
    q.push(2);

    EXPECT_EQ(q.size(), 2);
    EXPECT_EQ(q.pop(), 2);
    EXPECT_EQ(q.pop(), 1);
    EXPECT_EQ(q.size(), 0);
    EXPECT_TRUE(q.empty());
}

TEST(ThreadSafeQueueTests, ThreadSafeQueueTests_twoThreadsPopPush_Test)
{
    obe::ThreadSafeQueue<int> q;
    std::jthread threadPop([&](){
        EXPECT_EQ(q.pop(),1);
        EXPECT_EQ(q.pop(),2);
        EXPECT_EQ(q.pop(),3);
        EXPECT_EQ(q.pop(),4);
        EXPECT_EQ(q.pop(),5);
    });

    std::jthread threadPush([&](){
        q.push(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        q.push(2);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        q.push(3);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        q.push(4);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        q.push(5);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    });

}