#pragma once 
#include <print>
#include <mutex>

namespace obe
{
    template<typename T>
    class ThreadSafeQueue
    {
    public:
        ThreadSafeQueue(/* args */)        
        {
            head = new Node{};
            head->next = nullptr;
        }

        // TODO this needs to release all memory
        //~ThreadSafeQueue();

        void push(const T& data)
        {
            std::unique_lock<std::mutex>lock(mutex);
            auto newNode = new Node{};
            newNode->data = data;

            newNode->next = head;
            head = newNode;

            ready = true;
            cv.notify_one();
            ++_size;
        }

        bool tryPop(T* outOrder)
        {
            std::scoped_lock<std::mutex>lock(mutex);
            
            if (empty())
            {
                outOrder = pop();
                return true;
            }

            return false;
        }

        T pop()
        {
            std::unique_lock<std::mutex> lock(mutex);
            
            cv.wait_for(lock, std::chrono::milliseconds(5) ,[&](){
                return ready == true;
            });
             
            T data = head->data;
            auto next = head->next;
            
            delete head;
            head = nullptr;
            
            head = next;
            ready = false;

            --_size;
            
            return data;
        }

        std::size_t size() const
        {
            return _size;
        }

        bool empty()
        {
            return head->next == nullptr;
        }

    private:
        struct Node
        {
            T data;
            Node* next;
        };

        std::size_t _size{0};
        Node* head;
        std::mutex mutex;
        std::condition_variable cv;
        std::atomic<bool> ready; 

    };
}