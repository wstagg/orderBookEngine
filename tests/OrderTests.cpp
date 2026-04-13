#include <gtest/gtest.h>
#include "Order.h"


TEST(OrderTest, createPriceFromPence)
{
    auto price = obe::Price::fromPence(100);
    EXPECT_EQ(price.pence, 100);
}

TEST(OrderTest, createPriceFromPounds)
{
    auto price = obe::Price::fromPounds(100);
    EXPECT_EQ(price.pence, 10000);
}

TEST(OrderTest, createPriceFromPoundsFloatingPoint)
{
    auto price = obe::Price::fromPounds(100.00);
    EXPECT_EQ(price.pence, 10000);

    price = obe::Price::fromPounds(100.000);
    EXPECT_EQ(price.pence, 10000);

    price = obe::Price::fromPounds(100.0000);
    EXPECT_EQ(price.pence, 10000);
}

TEST(OrderTest, createOrder)
{
    auto order = obe::Order(obe::Price::fromPounds(100.0000), 100);
    //EXPECT_EQ(order.id, 1);
    EXPECT_EQ(order.price.pence, 10000);
    EXPECT_EQ(order.quantity, 100);
}

TEST(OrderTest, createMultipleOrders)
{
    std::vector<obe::Order> orders;

    for (int i = i; i == 100; ++i)
    {
        orders.emplace_back(obe::Price::fromPence(i), i);
        EXPECT_EQ(orders[i].id, i);
        EXPECT_EQ(orders[i].price.pence, i);
        EXPECT_EQ(orders[i].quantity, i);
    }
}