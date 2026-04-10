#include "MatchingEngine.h"

std::vector<TradeEvent> obe::MatchingEngine::submitOrder(const OrderType &orderType, const Order order)
{
    switch (orderType)
    {
    case OrderType::ask:
        asks.insert(order);
        break;
    case OrderType::bid:
        bids.insert(order);
    default:
        break;
    }


}

std::vector<TradeEvent> obe::MatchingEngine::matchingLoop()
{
    //while()
    return std::vector<TradeEvent>();
}
