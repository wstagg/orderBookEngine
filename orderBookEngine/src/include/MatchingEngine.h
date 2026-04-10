#pragma once
#include <vector>
#include "SortedOrderList.h"
#include "TradeEvent.h"

namespace obe
{
    class MatchingEngine
    {
    public:
        MatchingEngine() = default;
        std::vector<TradeEvent> submitOrder(const OrderType& orderType, const Order order);
    private:
        
        std::vector<TradeEvent> matchingLoop();
        obe::SortedOrderList<obe::AskComparator> asks;
        obe::SortedOrderList<obe::BidComparator> bids;
    }; 
}