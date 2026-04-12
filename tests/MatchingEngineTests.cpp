#include <gtest/gtest.h>
#include "MatchingEngine.h"

TEST(MatchingEngineTests, submitSingleAsk)
{
    obe::MatchingEngine engine;

    obe::Order ask{1, obe::Price::fromPounds(1.00), 100};
    
    auto trades = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_TRUE(trades.empty());
}

TEST(MatchingEngineTests, submitSingleBid)
{
    obe::MatchingEngine engine;

    obe::Order bid{1, obe::Price::fromPounds(1.00), 100};
    
    auto trades = engine.submitOrder(obe::OrderType::bid, bid);

    EXPECT_TRUE(trades.empty());
}

TEST(MatchingEngineTests, bidAndAskSamePriceSameQuatity)
{
    obe::MatchingEngine engine;

    obe::Order bid{1, obe::Price::fromPounds(1.00), 100};
    obe::Order ask{2, obe::Price::fromPounds(1.00), 100};
    
    auto trades1 = engine.submitOrder(obe::OrderType::bid, bid);

    EXPECT_TRUE(trades1.empty());

    auto trades2 = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_FALSE(trades2.empty());
    EXPECT_EQ(trades2.size(), 1);
    EXPECT_EQ(trades2.front().askId, 2);
    EXPECT_EQ(trades2.front().bidId, 1);
    EXPECT_EQ(trades2.front().quantity, 100);
    EXPECT_EQ(trades2.front().price.pence, 100);
}

TEST(MatchingEngineTests, askPriceLessQuantityEqual)
{
    obe::MatchingEngine engine;

    obe::Order bid{1, obe::Price::fromPounds(1.00), 100};
    obe::Order ask{2, obe::Price::fromPounds(0.99), 100};
    
    auto trades1 = engine.submitOrder(obe::OrderType::bid, bid);

    EXPECT_TRUE(trades1.empty());

    auto trades2 = engine.submitOrder(obe::OrderType::ask, ask);

    EXPECT_FALSE(trades2.empty());
    EXPECT_EQ(trades2.size(), 1);
    EXPECT_EQ(trades2.front().askId, 2);
    EXPECT_EQ(trades2.front().bidId, 1);
    EXPECT_EQ(trades2.front().quantity, 100);
    EXPECT_EQ(trades2.front().price.pence, 99);
}