#include <gtest/gtest.h>
#include "DynamicArray.h"
#include "Order.h"




TEST(DynamicArrayTests, DynamicArrayTests_arrayPush_Test)
{
    obe::DynamicArray<obe::Order> q;
    for (int i = 0 ; i < 500 ; ++i)
    {
        q.push({obe::Price::fromPence(i), i});
    }

    EXPECT_EQ(q.size(), 500);
    EXPECT_EQ(q[1].price.pence, 1);
    EXPECT_EQ(q[499].price.pence, 499);
}