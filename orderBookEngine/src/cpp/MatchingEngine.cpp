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
        break;
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
        while (asksList.peekBestOrder().value().price.pence <= bidsList.peekBestOrder().value().price.pence)
        {
            if (asksList.peekBestOrder().value().quantity == bidsList.peekBestOrder().value().quantity)
            {
                auto bid = bidsList.popBestOrder();
                auto ask = asksList.popBestOrder();

                tradeEvents.emplace_back(bid.value().id, ask.value().id, ask.value().price, ask.value().quantity);
            }

            else if (asksList.peekBestOrder().value().quantity > bidsList.peekBestOrder().value().quantity)
            {
                auto bid = bidsList.popBestOrder();
                
                tradeEvents.emplace_back(bid.value().id, asksList.peekBestOrder().value().id, asksList.peekBestOrder().value().price, bid.value().quantity);
                
                asksList.reduceOrderQuantity(asksList.peekBestOrder().value().id, (asksList.peekBestOrder().value().quantity - bid.value().quantity));
            }

            else if (asksList.peekBestOrder().value().quantity < bidsList.peekBestOrder().value().quantity)
            {
                auto ask = asksList.popBestOrder();
                
                tradeEvents.emplace_back(bidsList.peekBestOrder().value().id, ask.value().id, ask.value().price, ask.value().quantity);
                
                bidsList.reduceOrderQuantity(bidsList.peekBestOrder().value().id, (bidsList.peekBestOrder().value().quantity - ask.value().quantity));
            }

            // check to see if either list has become empty and break out of loop if true
            if (asksList.size() == 0 || bidsList.size() == 0)
            {
                break;
            }
        }   
    }
    return tradeEvents;
}
