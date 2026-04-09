#include <gtest/gtest.h>
#include "SortedOrderList.h"

class SortedOrderListFixture : public testing::Test
{
public:
    SortedOrderListFixture()
    {
        for (int i = 0 ; i < 100 ; ++i)
        {
            askOrderbook.insert({id, pricePence, quantity});
            bidOrderbook.insert({id, pricePence, quantity});
            ++id;
            ++pricePence;
            quantity;
        }
    }

    SortedOrderList<std::less<obe::Price::Pence>> askOrderbook;
    SortedOrderList<std::greater<obe::Price::Pence>> bidOrderbook;
    const int TOTAL_ORDERS{100};

private:

    int32_t id{1};
    int64_t pricePence{1};
    int32_t quantity{1};
};

TEST_F(SortedOrderListFixture, OrderBookInsert)
{
    EXPECT_EQ(SortedOrderListFixture::TOTAL_ORDERS, askOrderbook.getSize());
    EXPECT_EQ(SortedOrderListFixture::TOTAL_ORDERS, bidOrderbook.getSize());
}

TEST(SortedOrderListTests, getBestPriceAsk)
{
    SortedOrderList<std::less<obe::Price::Pence>> askOrderbook;
    int64_t highPrice{100};
    int64_t lowPrice{1};

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {1, lowPrice, 1};
    askOrderbook.insert(order1);
    askOrderbook.insert(order2);

    EXPECT_EQ(lowPrice, askOrderbook.getBestPrice());
}

TEST(SortedOrderListTests, getBestPriceBid)
{
    SortedOrderList<std::greater<obe::Price::Pence>> bidOrderbook;
    int64_t highPrice{100};
    int64_t lowPrice{1};

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {1, lowPrice, 1};
    bidOrderbook.insert(order1);
    bidOrderbook.insert(order2);

    EXPECT_EQ(highPrice, bidOrderbook.getBestPrice());
}

TEST(SortedOrderListTests, removeOrderSuccess)
{
    SortedOrderList<std::less<obe::Price::Pence>> askOrderbook;
    int64_t highPrice{100};
    int64_t lowPrice{1};

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 1};
    askOrderbook.insert(order1);
    askOrderbook.insert(order2);
    
    auto success = askOrderbook.removeOrder(1);

    EXPECT_EQ(success, true);
    EXPECT_EQ(askOrderbook.getSize(), 1); 
}

TEST(SortedOrderListTests, removeOrderFail)
{
    SortedOrderList<std::less<obe::Price::Pence>> askOrderbook;
    int64_t highPrice{100};
    int64_t lowPrice{1};

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 1};
    askOrderbook.insert(order1);
    askOrderbook.insert(order2);
    
    auto success = askOrderbook.removeOrder(3);

    EXPECT_EQ(success, false);
    EXPECT_EQ(askOrderbook.getSize(), 2); 
}

TEST(SortedOrderListTests, popBestAskOrder)
{
    SortedOrderList<std::less<obe::Price::Pence>> askOrderbook;
    int64_t highPrice{100};
    int64_t lowPrice{1};

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 1};
    askOrderbook.insert(order1);
    askOrderbook.insert(order2);
    
    auto order = askOrderbook.popBestOrder();

    EXPECT_EQ(order.id, 2);
    EXPECT_EQ(order.price.pence, lowPrice);
    EXPECT_EQ(order.quantity, 1);
    EXPECT_EQ(askOrderbook.getSize(), 1); 
}

TEST(SortedOrderListTests, popBestBidOrder)
{
    SortedOrderList<std::greater<obe::Price::Pence>> askOrderbook;
    int64_t highPrice{100};
    int64_t lowPrice{1};

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 1};
    askOrderbook.insert(order1);
    askOrderbook.insert(order2);
    
    auto order = askOrderbook.popBestOrder();

    EXPECT_EQ(order.id, 1);
    EXPECT_EQ(order.price.pence, highPrice);
    EXPECT_EQ(order.quantity, 1);
    EXPECT_EQ(askOrderbook.getSize(), 1); 
}