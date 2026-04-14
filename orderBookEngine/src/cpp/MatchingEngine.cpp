#include "MatchingEngine.h"

std::vector<obe::TradeEvent> obe::MatchingEngine::submitOrder(const obe::OrderType &orderType, const obe::Order& order)
{
    auto pOrder = memoryPool.allocate(std::move(order));

    // if no more space in the memoryPool return empty vector
    if (!pOrder) return {};
    
    switch (orderType)
    {
    case OrderType::ask:
        asksList.insert(pOrder);
        break;
    case OrderType::bid:
        bidsList.insert(pOrder);
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
    
    while ( asksList.peekBestOrder() != nullptr
            && bidsList.peekBestOrder() != nullptr
            && (asksList.peekBestOrder()->price.pence <= bidsList.peekBestOrder()->price.pence))
    {
        if (asksList.peekBestOrder()->quantity == bidsList.peekBestOrder()->quantity)
        {
            auto bid = bidsList.popBestOrder();
            auto ask = asksList.popBestOrder();

            tradeEvents.emplace_back(bid->id, ask->id, ask->price, ask->quantity);
            memoryPool.deallocate(bid);
            memoryPool.deallocate(ask);
        }

        else if (asksList.peekBestOrder()->quantity > bidsList.peekBestOrder()->quantity)
        {
            auto bid = bidsList.popBestOrder();

            tradeEvents.emplace_back(bid->id, asksList.peekBestOrder()->id, asksList.peekBestOrder()->price, bid->quantity);
            asksList.reduceOrderQuantity(asksList.peekBestOrder()->id, (asksList.peekBestOrder()->quantity - bid->quantity));
            memoryPool.deallocate(bid);
        }

        else if (asksList.peekBestOrder()->quantity < bidsList.peekBestOrder()->quantity)
        {
            auto ask = asksList.popBestOrder();
            
            tradeEvents.emplace_back(bidsList.peekBestOrder()->id, ask->id, ask->price, ask->quantity);
            bidsList.reduceOrderQuantity(bidsList.peekBestOrder()->id, (bidsList.peekBestOrder()->quantity - ask->quantity));
            memoryPool.deallocate(ask);
        }
    }   
    
    return tradeEvents;
}
