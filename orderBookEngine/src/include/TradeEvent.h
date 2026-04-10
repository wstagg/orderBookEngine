#pragma once
#include <cstdint>

#include "Order.h"

namespace obe
{
    struct TradeEvent
    {
        int64_t bidId;
        int64_t askId;
        Price price;
        int32_t quantity;
    }; 
}