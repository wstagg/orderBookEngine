#pragma once
#include <cstdint>
#include <cmath>

namespace obe
{
    struct Price
    {
        using Pence = int64_t;
        Price(int64_t _pence):
        pence(_pence)
        {
        }

        Price(double price)
        {
            pence = static_cast<int64_t>(std::round(price * 100));
        }

        Pence pence;
    };

    struct Order
    {
        int32_t id;
        Price price;
        int32_t quantity;
    };
    
}