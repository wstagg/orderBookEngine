#include <gtest/gtest.h>
#include "SortedOrderList.h"

class SortedOrderListFixture : public testing::Test
{
public:
    SortedOrderListFixture()
    {
        for (int i = 0 ; i < 100 ; ++i)
        {
            askOrderbook.insert({id, obe::Price::fromPence(pricePence), quantity});
            bidOrderbook.insert({id, obe::Price::fromPence(pricePence), quantity});
            ++id;
            ++pricePence;
            quantity;
        }
    }

    obe::SortedOrderList<obe::AskComparator> askOrderbook;
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
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
    obe::SortedOrderList<obe::AskComparator> askOrderbook;
    obe::Price highPrice = obe::Price::fromPence(100);
    obe::Price lowPrice = obe::Price::fromPence(1);

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 1};
    askOrderbook.insert(order1);
    askOrderbook.insert(order2);

    EXPECT_EQ(lowPrice.pence, askOrderbook.getBestPrice());
}

TEST(SortedOrderListTests, getBestPriceBid)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    obe::Price highPrice = obe::Price::fromPence(100);
    obe::Price lowPrice = obe::Price::fromPence(1);

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 1};
    bidOrderbook.insert(order1);
    bidOrderbook.insert(order2);

    EXPECT_EQ(highPrice.pence, bidOrderbook.getBestPrice());
}

TEST(SortedOrderListTests, getBestPriceOnEmptySortedOrderList)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    auto bestPrice = bidOrderbook.getBestPrice();

    EXPECT_EQ(std::nullopt, bestPrice);
}

TEST(SortedOrderListTests, removeOrderSuccess)
{
    obe::SortedOrderList<obe::AskComparator> askOrderbook;
    obe::Price highPrice = obe::Price::fromPence(100);
    obe::Price lowPrice = obe::Price::fromPence(1);

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
    obe::SortedOrderList<obe::AskComparator> askOrderbook;
    obe::Price highPrice = obe::Price::fromPence(100);
    obe::Price lowPrice = obe::Price::fromPence(1);

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
    obe::SortedOrderList<obe::AskComparator> askOrderbook;
    obe::Price highPrice = obe::Price::fromPence(100);
    obe::Price lowPrice = obe::Price::fromPence(1);

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 1};
    askOrderbook.insert(order1);
    askOrderbook.insert(order2);
    
    auto order = askOrderbook.popBestOrder();

    EXPECT_EQ(order.value().id, 2);
    EXPECT_EQ(order.value().price.pence, lowPrice.pence);
    EXPECT_EQ(order.value().quantity, 1);
    EXPECT_EQ(askOrderbook.getSize(), 1); 
}

TEST(SortedOrderListTests, popBestBidOrder)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    obe::Price highPrice = obe::Price::fromPence(100);
    obe::Price lowPrice = obe::Price::fromPence(1);

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 1};
    bidOrderbook.insert(order1);
    bidOrderbook.insert(order2);
    
    auto order = bidOrderbook.popBestOrder();

    EXPECT_EQ(order.value().id, 1);
    EXPECT_EQ(order.value().price.pence, highPrice.pence);
    EXPECT_EQ(order.value().quantity, 1);
    EXPECT_EQ(bidOrderbook.getSize(), 1); 
}

TEST(SortedOrderListTests, popBestOrderOnEmptyOrderList)
{
    obe::SortedOrderList<obe::AskComparator> askOrderbook;
    auto order = askOrderbook.popBestOrder();

    EXPECT_EQ(order, std::nullopt);
}