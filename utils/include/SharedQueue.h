#pragma once

#include <queue>
#include <mutex>

namespace utils {
    template<class T, class Container=std::deque<T> >
    class SharedQueue : private std::queue<T, Container>{
    private:
        std::mutex m;
    public:
        bool empty(){
            std::lock_guard<std::mutex> guard(m);
            return std::queue<T,Container>::empty();
        }
        size_t size(){
            std::lock_guard<std::mutex> guard(m);
            return std::queue<T,Container>::size();
        }
        void push(T &val){
            std::lock_guard<std::mutex> guard(m);
            std::queue<T,Container>::push(val);
        }
        void push(const T &val){
            std::lock_guard<std::mutex> guard(m);
            std::queue<T,Container>::push(val);
        }
        T pop(){
            std::lock_guard<std::mutex> guard(m);
            if(std::queue<T,Container>::empty()) throw std::logic_error("shared_queue::pop : no element to pop");
            T res = std::queue<T,Container>::front();
            std::queue<T,Container>::pop();
            return res;
        }
    };
}
