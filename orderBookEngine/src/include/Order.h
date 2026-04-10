#pragma once
#include <cstdint>
#include <cmath>

namespace obe
{
    class Price
    {
    public:
        using Pence = int64_t;
        
        static Price fromPence(Pence pence)
        {
            Price p;
            p.pence = pence;
            return p;
        }
    
        static Price fromPounds(double pounds)
        {
            Price p;
            p.pence = static_cast<Pence>(std::round(pounds * 1000));
            return p;
        }
        
        Pence pence;
    private:

        Price() = default;
    };

    struct Order
    {
        int64_t id;
        Price price;
        int32_t quantity;
    }; 

    enum class OrderType{ask, bid};
}