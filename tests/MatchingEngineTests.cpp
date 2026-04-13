#include <gtest/gtest.h>
#include "MatchingEngine.h"

TEST(MatchingEngineTests, submitSingleAsk)
{
    obe::MatchingEngine engine;

    obe::Order ask{obe::Price::fromPounds(1.00), 100};
    
    auto trades = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_TRUE(trades.empty());
}

TEST(MatchingEngineTests, submitSingleBid)
{
    obe::MatchingEngine engine;

    obe::Order bid{obe::Price::fromPounds(1.00), 100};
    
    auto trades = engine.submitOrder(obe::OrderType::bid, bid);

    EXPECT_TRUE(trades.empty());
}

TEST(MatchingEngineTests, bidPriceLessThanAsk)
{
    obe::MatchingEngine engine;

    obe::Order bid{obe::Price::fromPounds(0.99), 100};
    obe::Order ask{obe::Price::fromPounds(1.00), 100};
    
    auto trades1 = engine.submitOrder(obe::OrderType::bid, bid);

    EXPECT_TRUE(trades1.empty());

    auto trades2 = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_TRUE(trades2.empty());
}

TEST(MatchingEngineTests, bidAndAskSamePriceSameQuatity)
{
    obe::MatchingEngine engine;

    obe::Order bid{obe::Price::fromPounds(1.00), 100};
    obe::Order ask{obe::Price::fromPounds(1.00), 100};
    
    auto trades1 = engine.submitOrder(obe::OrderType::bid, bid);

    EXPECT_TRUE(trades1.empty());

    auto trades2 = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_FALSE(trades2.empty());
    EXPECT_EQ(trades2.size(), 1);
    EXPECT_EQ(trades2.front().askId, ask.id);
    EXPECT_EQ(trades2.front().bidId, bid.id);
    EXPECT_EQ(trades2.front().quantity, 100);
    EXPECT_EQ(trades2.front().price.pence, 100);
}

TEST(MatchingEngineTests, askPriceLessQuantityEqual)
{
    obe::MatchingEngine engine;

    obe::Order bid{obe::Price::fromPounds(1.00), 100};
    obe::Order ask{obe::Price::fromPounds(0.99), 100};
    
    auto trades1 = engine.submitOrder(obe::OrderType::bid, bid);

    EXPECT_TRUE(trades1.empty());

    auto trades2 = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_FALSE(trades2.empty());
    EXPECT_EQ(trades2.size(), 1);
    EXPECT_EQ(trades2.front().askId, ask.id);
    EXPECT_EQ(trades2.front().bidId, bid.id);
    EXPECT_EQ(trades2.front().quantity, 100);
    EXPECT_EQ(trades2.front().price.pence, 99);
}

TEST(MatchingEngineTests, askQuantityGreaterThanBidQuantityPartialOrderFill)
{
    obe::MatchingEngine engine;

    obe::Order bid{obe::Price::fromPounds(1.00), 100};
    obe::Order ask{obe::Price::fromPounds(1.00), 200};
    
    engine.submitOrder(obe::OrderType::bid, bid);
    auto trades = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_EQ(trades.size(), 1);

    EXPECT_EQ(trades.front().askId, ask.id);
    EXPECT_EQ(trades.front().bidId, bid.id);
    EXPECT_EQ(trades.front().quantity, 100);
    EXPECT_EQ(trades.front().price.pence, 100);
}

TEST(MatchingEngineTests, askQuantityGreaterThanBidQuantityFullOrderFill)
{
    obe::MatchingEngine engine;

    obe::Order bid1{obe::Price::fromPounds(1.00), 100};
    obe::Order bid2{obe::Price::fromPounds(1.00), 100};
    obe::Order ask{obe::Price::fromPounds(1.00), 200};
    
    engine.submitOrder(obe::OrderType::bid, bid1);
    engine.submitOrder(obe::OrderType::bid, bid2);
    auto trades = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_EQ(trades.size(), 2);

    EXPECT_EQ(trades[0].askId, ask.id);
    EXPECT_EQ(trades[0].bidId, bid1.id);
    EXPECT_EQ(trades[0].quantity, 100);
    EXPECT_EQ(trades[0].price.pence, 100);

    EXPECT_EQ(trades[1].askId, ask.id);
    EXPECT_EQ(trades[1].bidId, bid2.id);
    EXPECT_EQ(trades[1].quantity, 100);
    EXPECT_EQ(trades[1].price.pence, 100);
}

TEST(MatchingEngineTests, bidQuantityGreaterThanAskQuantityPartialOrderFill)
{
    obe::MatchingEngine engine;

    obe::Order bid{obe::Price::fromPounds(1.00), 200};
    obe::Order ask{obe::Price::fromPounds(1.00), 100};
    
    engine.submitOrder(obe::OrderType::bid, bid);
    auto trades = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_EQ(trades.size(), 1);

    EXPECT_EQ(trades.front().askId, ask.id);
    EXPECT_EQ(trades.front().bidId, bid.id);
    EXPECT_EQ(trades.front().quantity, 100);
    EXPECT_EQ(trades.front().price.pence, 100);
}

TEST(MatchingEngineTests, bidQuantityGreaterThanAskQuantityFullOrderFill)
{
    obe::MatchingEngine engine;

    obe::Order bid1{obe::Price::fromPounds(1.00), 200};
    obe::Order ask2{obe::Price::fromPounds(1.00), 100};
    obe::Order ask1{obe::Price::fromPounds(1.00), 100};
    
    engine.submitOrder(obe::OrderType::ask, ask1);
    engine.submitOrder(obe::OrderType::ask, ask2);
    auto trades = engine.submitOrder(obe::OrderType::bid, bid1);

    EXPECT_EQ(trades.size(), 2);

    EXPECT_EQ(trades[0].askId, ask1.id);
    EXPECT_EQ(trades[0].bidId, bid1.id);
    EXPECT_EQ(trades[0].quantity, 100);
    EXPECT_EQ(trades[0].price.pence, 100);

    EXPECT_EQ(trades[1].askId, ask2.id);
    EXPECT_EQ(trades[1].bidId, bid1.id);
    EXPECT_EQ(trades[1].quantity, 100);
    EXPECT_EQ(trades[1].price.pence, 100);
}

TEST(MatchingEngineTests, askQuantityGreaterThanBidQuantityPartialOrderFillBidGreaterPrice)
{
    obe::MatchingEngine engine;

    obe::Order bid{obe::Price::fromPounds(2.00), 100};
    obe::Order ask{obe::Price::fromPounds(1.00), 200};
    
    engine.submitOrder(obe::OrderType::bid, bid);
    auto trades = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_EQ(trades.size(), 1);

    EXPECT_EQ(trades.front().askId, ask.id);
    EXPECT_EQ(trades.front().bidId, bid.id);
    EXPECT_EQ(trades.front().quantity, 100);
    EXPECT_EQ(trades.front().price.pence, 100);
}

TEST(MatchingEngineTests, askQuantityGreaterThanBidQuantityFullOrderFillBidGreaterPrice)
{
    obe::MatchingEngine engine;

    obe::Order bid1{obe::Price::fromPounds(2.00), 100};
    obe::Order bid2{obe::Price::fromPounds(2.00), 100};
    obe::Order ask{obe::Price::fromPounds(1.00), 200};
    
    engine.submitOrder(obe::OrderType::bid, bid1);
    engine.submitOrder(obe::OrderType::bid, bid2);
    auto trades = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_EQ(trades.size(), 2);

    EXPECT_EQ(trades[0].askId, ask.id);
    EXPECT_EQ(trades[0].bidId, bid1.id);
    EXPECT_EQ(trades[0].quantity, 100);
    EXPECT_EQ(trades[0].price.pence, 100);

    EXPECT_EQ(trades[1].askId, ask.id);
    EXPECT_EQ(trades[1].bidId, bid2.id);
    EXPECT_EQ(trades[1].quantity, 100);
    EXPECT_EQ(trades[1].price.pence, 100);
}

TEST(MatchingEngineTests, bidQuantityGreaterThanAskQuantityPartialOrderFillBidGreaterPrice)
{
    obe::MatchingEngine engine;

    obe::Order bid{obe::Price::fromPounds(2.00), 200};
    obe::Order ask{obe::Price::fromPounds(1.00), 100};
    
    engine.submitOrder(obe::OrderType::bid, bid);
    auto trades = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_EQ(trades.size(), 1);

    EXPECT_EQ(trades.front().askId, ask.id);
    EXPECT_EQ(trades.front().bidId, bid.id);
    EXPECT_EQ(trades.front().quantity, 100);
    EXPECT_EQ(trades.front().price.pence, 100);
}

TEST(MatchingEngineTests, bidQuantityGreaterThanAskQuantityFullOrderFillBidGreaterPrice)
{
    obe::MatchingEngine engine;

    obe::Order bid1{obe::Price::fromPounds(2.00), 200};
    obe::Order ask2{obe::Price::fromPounds(1.00), 100};
    obe::Order ask1{obe::Price::fromPounds(1.00), 100};
    
    engine.submitOrder(obe::OrderType::ask, ask1);
    engine.submitOrder(obe::OrderType::ask, ask2);
    auto trades = engine.submitOrder(obe::OrderType::bid, bid1);

    EXPECT_EQ(trades.size(), 2);

    EXPECT_EQ(trades[0].askId, ask1.id);
    EXPECT_EQ(trades[0].bidId, bid1.id);
    EXPECT_EQ(trades[0].quantity, 100);
    EXPECT_EQ(trades[0].price.pence, 100);

    EXPECT_EQ(trades[1].askId, ask2.id);
    EXPECT_EQ(trades[1].bidId, bid1.id);
    EXPECT_EQ(trades[1].quantity, 100);
    EXPECT_EQ(trades[1].price.pence, 100);
}

