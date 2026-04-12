#pragma once
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cassert>
#include <optional>

#include "Order.h"
namespace obe
{
    using BidComparator = std::greater<Price::Pence>;
    using AskComparator = std::less<Price::Pence>;

    template<typename sortOrder>
    class SortedOrderList
    {
    public:
        SortedOrderList() = default;
        
        void insert(const obe::Order& order)
        {
            priceToOrdersMap[order.price.pence].push_back(order);
            idToPriceMap[order.id] = order.price.pence;
            ++totalOrders;
            assert((totalOrders == idToPriceMap.size()) && "totalOrders and idToPriceMap.size() not equal");
        }

        int64_t size() const 
        {
            return totalOrders;
        }
        
        const std::optional<obe::Order> peekBestOrder() const
        {
            if(priceToOrdersMap.empty())
            {
                return std::nullopt;
            }

            return priceToOrdersMap.begin()->second.front();
        }
        
        bool remove(const int64_t orderId)
        {
            const auto orderPrice = idToPriceMap.find(orderId);

            // no order id exists
            if (orderPrice == idToPriceMap.end()) return false;

            const auto orderQueue = priceToOrdersMap.find(orderPrice->second);

            //price does not exist
            if (orderQueue == priceToOrdersMap.end()) return false;

            for (auto itt = orderQueue->second.begin(); itt != orderQueue->second.end(); )
            {
                if(itt->id == orderId)
                {
                    //erase the order from the queue
                    orderQueue->second.erase(itt);
                    
                    //erase the ID from the idToPriceMap
                    idToPriceMap.erase(orderId);
                    
                    // if the queue of orders at the price is now empty remove the price from priceToOrdersMap
                    if (orderQueue->second.empty())
                    {
                        priceToOrdersMap.erase(orderPrice->second);
                    }
                    
                    --totalOrders;
                    assert((totalOrders == idToPriceMap.size()) && "totalOrders and idToPriceMap.size() not equal");
                    return true;
                }
            }
            
            return false;
        }

        std::optional<obe::Order> popBestOrder()
        {
            if(priceToOrdersMap.empty())
            {
                return std::nullopt;
            }
            auto order = priceToOrdersMap.begin()->second.front();
            remove(order.id);
            assert((totalOrders == idToPriceMap.size()) && "totalOrders and idToPriceMap.size() not equal");
            return order;
        }

        bool reduceOrderQuantity(int64_t orderId, int32_t newQuantity)
        {
            const auto orderPrice = idToPriceMap.find(orderId);

            // no order id exists
            if (orderPrice == idToPriceMap.end()) return false;

            const auto orderQueue = priceToOrdersMap.find(orderPrice->second);

            //price does not exist
            if (orderQueue == priceToOrdersMap.end()) return false;

            for (auto itt = orderQueue->second.begin(); itt != orderQueue->second.end(); )
            {
                if(itt->id == orderId)
                {
                    if (newQuantity < itt->quantity)
                    {
                        itt->quantity = newQuantity;
                        return true;
                    }
                    break;
                }
            }
            return false;
        }

    private:
        std::map<int64_t, std::vector<obe::Order>, sortOrder> priceToOrdersMap; 
        std::unordered_map<int32_t, int64_t> idToPriceMap; 
        uint64_t totalOrders{0};
    };
}