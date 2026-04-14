#pragma once
#include <vector>
#include "SortedOrderList.h"
#include "TradeEvent.h"
#include "MemoryPool.h"

namespace obe
{
    class MatchingEngine
    {
    public:
        MatchingEngine() = default;
        std::vector<TradeEvent> submitOrder(const obe::OrderType& orderType, const obe::Order& order);
    private:
        
        std::vector<obe::TradeEvent> matchingLoop();
        obe::SortedOrderList<obe::AskComparator> asksList;
        obe::SortedOrderList<obe::BidComparator> bidsList;
        
        const static std::size_t POOL_SIZE{10000};
        obe::MemoryPool<obe::Order, POOL_SIZE> memoryPool;
    }; 
}