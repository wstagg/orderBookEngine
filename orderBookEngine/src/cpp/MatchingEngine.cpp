#include "MatchingEngine.h"

std::vector<obe::TradeEvent> obe::MatchingEngine::submitOrder(const obe::OrderType &orderType, const obe::Order& order)
{
    switch (orderType)
    {
    case OrderType::ask:
        asksList.insert(order);
        break;
    case OrderType::bid:
        bidsList.insert(order);
    default:
        break;
    }

    return matchingLoop();
}

std::vector<obe::TradeEvent> obe::MatchingEngine::matchingLoop()
{
    std::vector<obe::TradeEvent> tradeEvents{};
    
    // make sure both lists are not empty
    if (asksList.size() > 0 && bidsList.size() > 0)
    {
        while(asksList.peekBestOrder().value().price.pence <= bidsList.peekBestOrder().value().price.pence)
        {
            if (asksList.peekBestOrder().value().quantity == bidsList.peekBestOrder().value().quantity)
            {
                auto bid = bidsList.popBestOrder();
                auto ask = asksList.popBestOrder();

                tradeEvents.emplace_back(bid.value().id, ask.value().id, ask.value().price, ask.value().quantity);
            }

            // check to see if either list has become empty and break out if true
            if (asksList.size() == 0 || bidsList.size() == 0)
            {
                break;
            }
        }
        
        
        
    }
    return tradeEvents;
}
