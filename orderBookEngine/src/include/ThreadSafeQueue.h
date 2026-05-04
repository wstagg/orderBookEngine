#pragma once 
#include <print>
#include <mutex>
#include <optional>

namespace obe
{
    template<typename T>
    class ThreadSafeQueue
    {
    public:
        ThreadSafeQueue(/* args */)        
        {
        }

        // TODO this needs to release all memory
        ~ThreadSafeQueue()
        {
            stop();
            // clean up memory
            while (head != nullptr)
            {
                auto next = head->next;
                delete head;
                head = nullptr;
                head = next;
            } 
        }

        void stop()
        {
            std::scoped_lock<std::mutex>lock(mutex);
            kill = true;
            cv.notify_all();
        }


        void push(const T& data)
        {
            std::scoped_lock<std::mutex>lock(mutex);
            auto newNode = new Node{data, nullptr};

            if (head == nullptr)
            {
                head = newNode;
                tail = head;
                ++_size;
                cv.notify_one();
                return;
            }

            tail->next = newNode;
            tail = newNode;

            cv.notify_one();
            ++_size;
        }

        bool tryPop(T* out)
        {    
            if (!empty())
            {
                std::scoped_lock<std::mutex> lock(mutex);       
                *out = head->data;
                auto next = head->next;
                delete head;
                head = nullptr;
                head = next;
                --_size;
                return true;
            }

            return false;
        }

        std::optional<T> pop()
        {
            std::unique_lock<std::mutex> lock(mutex);
            
            cv.wait(lock,[&](){
                return head != nullptr || kill;
            });

            // only return nullopt when queue is empty
            if (kill && head == nullptr) 
            {
                return std::nullopt;
            }

            T data = head->data;
            auto next = head->next;
            delete head;
            head = nullptr;
            head = next;
            --_size;
            
            return data;
        }

        std::size_t size() const
        {
            return _size;
        }
        
        bool empty()
        {
            std::scoped_lock<std::mutex> lock(mutex);
            return head == nullptr;
        }

    private:
        struct Node
        {
            T data;
            Node* next;
        };

        std::size_t _size{0};
        Node* head = nullptr;
        Node* tail = nullptr;
        std::mutex mutex;
        std::condition_variable cv;
        bool kill{false}; 
    };
}