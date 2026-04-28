#pragma once
#include <cstdint>
#include <cmath>
#include <atomic>

namespace obe
{
    class Price
    {
    public:
        using Pence = uint64_t;
        
        static Price fromPence(Pence pence)
        {
            Price p;
            p.pence = pence;
            return p;
        }
    
        static Price fromPounds(double pounds)
        {
            Price p;
            p.pence = static_cast<Pence>(std::round(pounds * 100));
            return p;
        }
        
        Pence pence;
        Price() = default;
    private:

    };

    class Order
    {
    public:
        uint64_t id;
        Price price;
        int32_t quantity;

        Order() = default;
        Order(const Price& _price, int32_t _quantity):
        price(_price),
        quantity(_quantity)        
        {
            static std::atomic<uint64_t> _id{0};
            id = ++_id;
        }
    private:
        
    }; 

    enum class OrderType{ask, bid};
}