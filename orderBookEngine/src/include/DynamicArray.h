#pragma once
#include <queue>
#include <cstring>
#include <print>
#include <new>

namespace obe
{
    template<typename T>
    class DynamicArray
    {
    public:

        DynamicArray() = default;
        ~DynamicArray()
        {
            delete [] queue;
            queue = nullptr;
        }

        void push(const T& d)
        {
            // TODO should checkResize be before or after setting the index  
            queue[currentIndex] = d;
            ++currentIndex;
            checkResize();
        }

        std::size_t size() const
        {
            return std::distance(queue, &queue[currentIndex]);
        }

        T operator[](const std::size_t i)
        {
            return queue[i];
        }

    private:
        void checkResize()
        {
            // TODO is this right? 
            if (currentIndex == capacity)
            {
                T* temp {new T[capacity * 2]};
                memmove(temp, queue, currentIndex * sizeof(T));
                delete [] queue;
                queue = temp;
                //delete  [] temp;
                capacity *= 2;
                temp = nullptr;
            }
        }

        std::size_t currentIndex{0};
        std::size_t capacity{1};
        T* queue{new T[capacity]};
    };
}
