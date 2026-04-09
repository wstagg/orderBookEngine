#pragma once
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>

#include "Order.h"

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
    }

    const int getSize() const {return totalOrders;}
    
    const obe::Price::Pence getBestPrice() const
    {
        return priceToOrdersMap.begin()->first;
    }
    
    bool removeOrder(const int32_t& orderId)
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
                orderQueue->second.erase(itt);
                idToPriceMap.erase(orderId);
                // should orderPrice be removed from priceToOrdersMap if vector empty?
                --totalOrders;
                return true;
            }
        }
        
        return false;
    }

    obe::Order popBestOrder()
    {
        auto order = priceToOrdersMap.begin()->second.front();
        removeOrder(order.id);
        return order;
    }
    // Insert an order DONE
    // Peek at the best price (top of the list) without removing it DONE
    // Remove an order by its id — this is the cancellation case DONE
    // Remove the best order — this is the fill case, when a trade happens
    // Iterate in price order — the matching engine will need to walk the list

private:
    std::map<int64_t, std::vector<obe::Order>, sortOrder> priceToOrdersMap; 
    std::unordered_map<int32_t, int64_t> idToPriceMap; 
    int64_t totalOrders{0};
};