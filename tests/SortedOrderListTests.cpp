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
    EXPECT_EQ(SortedOrderListFixture::TOTAL_ORDERS, askOrderbook.size());
    EXPECT_EQ(SortedOrderListFixture::TOTAL_ORDERS, bidOrderbook.size());
}

TEST(SortedOrderListTests, peekBestOrderAsk)
{
    obe::SortedOrderList<obe::AskComparator> askOrderbook;
    obe::Price highPrice = obe::Price::fromPence(100);
    obe::Price lowPrice = obe::Price::fromPence(1);

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 2};
    askOrderbook.insert(order1);
    askOrderbook.insert(order2);

    auto order = askOrderbook.peekBestOrder();

    ASSERT_TRUE(order.has_value());

    EXPECT_EQ(lowPrice.pence, order.value().price.pence);
    EXPECT_EQ(2, order.value().id);
    EXPECT_EQ(2, order.value().quantity);
}

TEST(SortedOrderListTests, peekBestOrderBid)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    obe::Price highPrice = obe::Price::fromPence(100);
    obe::Price lowPrice = obe::Price::fromPence(1);

    obe::Order order1 {1, highPrice, 1};
    obe::Order order2 {2, lowPrice, 1};
    bidOrderbook.insert(order1);
    bidOrderbook.insert(order2);

    auto order = bidOrderbook.peekBestOrder();

    ASSERT_TRUE(order.has_value());

    EXPECT_EQ(highPrice.pence, order.value().price.pence);
    EXPECT_EQ(1, order.value().id);
    EXPECT_EQ(1, order.value().quantity);
}

TEST(SortedOrderListTests, getBestPriceOnEmptySortedOrderList)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    auto bestPrice = bidOrderbook.peekBestOrder();

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
    
    auto success = askOrderbook.remove(1);

    EXPECT_EQ(success, true);
    EXPECT_EQ(askOrderbook.size(), 1); 
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
    
    auto success = askOrderbook.remove(3);

    EXPECT_EQ(success, false);
    EXPECT_EQ(askOrderbook.size(), 2); 
}

TEST(SortedOrderListTests, removeOrderFromEmptyOrderList)
{
    obe::SortedOrderList<obe::AskComparator> askOrderbook;
    EXPECT_EQ(askOrderbook.remove(1), false);
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
    EXPECT_EQ(askOrderbook.size(), 1); 
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
    EXPECT_EQ(bidOrderbook.size(), 1); 
}

TEST(SortedOrderListTests, popBestOrderOnEmptyOrderList)
{
    obe::SortedOrderList<obe::AskComparator> askOrderbook;
    auto order = askOrderbook.popBestOrder();

    EXPECT_EQ(order, std::nullopt);
}

TEST(SortedOrderListTests, popBestOrderFromDuplicatePriceOrders)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    obe::Price price = obe::Price::fromPence(100);

    obe::Order order1 {1, price, 1};
    obe::Order order2 {2, price, 2};
    bidOrderbook.insert(order1);
    bidOrderbook.insert(order2);
    
    auto orderOne = bidOrderbook.popBestOrder();

    EXPECT_EQ(orderOne.value().id, 1);
    EXPECT_EQ(orderOne.value().price.pence, price.pence);
    EXPECT_EQ(orderOne.value().quantity, 1);
    EXPECT_EQ(bidOrderbook.size(), 1); 

    auto orderTwo = bidOrderbook.popBestOrder();

    EXPECT_EQ(orderTwo.value().id, 2);
    EXPECT_EQ(orderTwo.value().price.pence, price.pence);
    EXPECT_EQ(orderTwo.value().quantity, 2);
    EXPECT_EQ(bidOrderbook.size(), 0); 
}

TEST(SortedOrderListTests, getBestPriceAfterOrderRemoved)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    obe::Price priceOne = obe::Price::fromPence(100);
    obe::Price priceTwo = obe::Price::fromPence(99);

    obe::Order order1 {1, priceOne, 1};
    obe::Order order2 {2, priceTwo, 2};
    bidOrderbook.insert(order1);
    bidOrderbook.insert(order2);

    auto orderOne = bidOrderbook.peekBestOrder();
    
    ASSERT_TRUE(orderOne.has_value());

    EXPECT_EQ(priceOne.pence, orderOne.value().price.pence);
    EXPECT_EQ(1, orderOne.value().id);
    EXPECT_EQ(1, orderOne.value().quantity);

    ASSERT_TRUE(bidOrderbook.remove(1));

    auto orderTwo = bidOrderbook.peekBestOrder();
    
    ASSERT_TRUE(orderTwo.has_value());
    
    EXPECT_EQ(priceTwo.pence, orderTwo.value().price.pence);
    EXPECT_EQ(2, orderTwo.value().id);
    EXPECT_EQ(2, orderTwo.value().quantity);
}

TEST(SortedOrderListTests, reduceOrderQuantity)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    obe::Price price = obe::Price::fromPence(100);

    obe::Order order1 {1, price, 100};
    bidOrderbook.insert(order1);

    auto orderPeek = bidOrderbook.peekBestOrder();
    ASSERT_TRUE(orderPeek.has_value());

    EXPECT_EQ(orderPeek.value().quantity, 100);

    EXPECT_EQ(bidOrderbook.reduceOrderQuantity(1, 99), true);

    orderPeek = bidOrderbook.peekBestOrder();
    ASSERT_TRUE(orderPeek.has_value());

    EXPECT_EQ(orderPeek.value().quantity, 99);  
}

TEST(SortedOrderListTests,reduceOrderQuantityOrderQuantityHigherThanOriginal)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    obe::Price price = obe::Price::fromPence(100);

    obe::Order order1 {1, price, 100};
    bidOrderbook.insert(order1);

    auto orderPeek = bidOrderbook.peekBestOrder();
    ASSERT_TRUE(orderPeek.has_value());

    EXPECT_EQ(orderPeek.value().quantity, 100);

    EXPECT_EQ(bidOrderbook.reduceOrderQuantity(1, 101), false);

    orderPeek = bidOrderbook.peekBestOrder();
    ASSERT_TRUE(orderPeek.has_value());

    EXPECT_EQ(orderPeek.value().quantity, 100);  
}

TEST(SortedOrderListTests, reduceOrderQuantityInvalidOrderID)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    obe::Price price = obe::Price::fromPence(100);

    obe::Order order1 {1, price, 100};
    bidOrderbook.insert(order1);

    auto orderPeek = bidOrderbook.peekBestOrder();
    ASSERT_TRUE(orderPeek.has_value());

    EXPECT_EQ(orderPeek.value().quantity, 100);

    EXPECT_EQ(bidOrderbook.reduceOrderQuantity(2, 99), false);

    orderPeek = bidOrderbook.peekBestOrder();
    ASSERT_TRUE(orderPeek.has_value());

    EXPECT_EQ(orderPeek.value().quantity, 100); 
}

TEST(SortedOrderListTests, reduceOrderQuantityOrderQuantityEqual)
{
    obe::SortedOrderList<obe::BidComparator> bidOrderbook;
    obe::Price price = obe::Price::fromPence(100);

    obe::Order order1 {1, price, 100};
    bidOrderbook.insert(order1);

    auto orderPeek = bidOrderbook.peekBestOrder();
    ASSERT_TRUE(orderPeek.has_value());

    EXPECT_EQ(orderPeek.value().quantity, 100);

    EXPECT_EQ(bidOrderbook.reduceOrderQuantity(1, 100), false);
}