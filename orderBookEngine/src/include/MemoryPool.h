#pragma once
#include <cstddef>
#include <utility>

namespace obe
{
    template<typename T, std::size_t capacity>
    class MemoryPool
    {
    public:
        MemoryPool()
        {
            for(std::size_t i = 0; i < capacity; ++i)
            {
                storage[i].next = data(i + 1);
            }
            storage[capacity - 1].next = nullptr;
            head.next = data(0);
        }

        [[nodiscard]]T* allocate(const T&& inData)  
        {
            if (head.next == nullptr)
            {
                return nullptr;
            }

            auto next = head.next->next;
            auto d = reinterpret_cast<T*>(head.next);
            d = new (d) T(std::move(inData));
            head.next = next;
            return d;
        }

        [[nodiscard]]T* allocate(const T& inData)  
        {
            if (head.next == nullptr)
            {
                return nullptr;
            }

            auto next = head.next->next;
            auto d = reinterpret_cast<T*>(head.next);
            d = new (d) T(inData);
            head.next = next;
            return d;
        }

        void deallocate(T* x)
        {
            x->~T();
            auto newData = reinterpret_cast<Data*>(x);
            newData->next = head.next;
            head.next = newData;
        }

    private:
        union Data
        {
            alignas(T) std::byte data[sizeof(T)];
            Data* next;
        };

        Data storage[capacity];
        Data head;
        
        Data* data(const std::size_t i)
        {
            return &storage[i];
        };
    };
}