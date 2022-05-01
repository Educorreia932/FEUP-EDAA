#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace utils {
    template<class T, class Container=std::deque<T> >
    class SharedQueue : private std::queue<T, Container>{
    private:
        std::mutex m;
        std::condition_variable cv;
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
            cv.notify_all();
        }
        void push(const T &val){
            std::lock_guard<std::mutex> guard(m);
            std::queue<T,Container>::push(val);
            cv.notify_all();
        }
        T pop(){
            std::lock_guard<std::mutex> guard(m);
            if(std::queue<T,Container>::empty()) throw std::logic_error("shared_queue::pop : no element to pop");
            T res = std::queue<T,Container>::front();
            std::queue<T,Container>::pop();
            return res;
        }
        T popWait(){
            std::unique_lock<std::mutex> lk(m);
            while(std::queue<T,Container>::empty()){
                cv.wait(lk);
            }
            T res = std::queue<T,Container>::front();
            std::queue<T,Container>::pop();
            return res;
        }
    };
}
