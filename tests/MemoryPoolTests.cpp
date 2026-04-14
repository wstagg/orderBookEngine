#include <gtest/gtest.h>
#include "MemoryPool.h"
#include "Order.h"

struct Vector_3f
{
    double x;
    double y;
    double z;
};

TEST(MemoryPoolTests, TestAllocationRvalueReference)
{
    obe::MemoryPool<obe::Order, 1> memoryPool;

    auto order = memoryPool.allocate({obe::Price::fromPence(100), 100});
    EXPECT_NE(order, nullptr);
    EXPECT_EQ(order->price.pence, 100);
    EXPECT_EQ(order->quantity, 100);
}

TEST(MemoryPoolTests, TestAllocationReference)
{
    obe::MemoryPool<obe::Order, 1> memoryPool;

    obe::Order order(obe::Price::fromPence(200), 200);
    
    auto pOrder = memoryPool.allocate(order);
    
    EXPECT_NE(pOrder, nullptr);
    EXPECT_EQ(pOrder->id, order.id);
    EXPECT_EQ(pOrder->price.pence, 200);
    EXPECT_EQ(pOrder->quantity, 200);

    EXPECT_NE(&order, pOrder);
    EXPECT_EQ(order.price.pence, 200);
    EXPECT_EQ(order.quantity, 200);
}

TEST(MemoryPoolTests, TestAllocatingOverCapicity)
{
    obe::MemoryPool<Vector_3f, 1> memoryPool;

    auto a = memoryPool.allocate({1.0, 2.0, 3.0});
    EXPECT_EQ(a->x, 1.0);
    EXPECT_EQ(a->y, 2.0);
    EXPECT_EQ(a->z, 3.0);
    EXPECT_NE(a, nullptr);

    auto b = memoryPool.allocate({1.0, 2.0, 3.0});
    EXPECT_EQ(b, nullptr);
}

TEST(MemoryPoolTests, TestDeallocating)
{
    obe::MemoryPool<Vector_3f, 1> memoryPool;

    auto a = memoryPool.allocate({1.0, 2.0, 3.0});
    EXPECT_EQ(a->x, 1.0);
    EXPECT_EQ(a->y, 2.0);
    EXPECT_EQ(a->z, 3.0);
    
    memoryPool.deallocate(a);
    
    auto b = memoryPool.allocate({4.0, 5.0, 6.0});

    // pointers should match as now using same memory address as deallocated
    EXPECT_EQ(a, b);

    EXPECT_EQ(b->x, 4.0);
    EXPECT_EQ(b->y, 5.0);
    EXPECT_EQ(b->z, 6.0);   

}