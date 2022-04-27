#pragma once

#include <list>
#include <thread>
#include <mutex>
#include <queue>

#include "SharedQueue.h"

namespace utils {
    class ThreadPool {
    public:
        class Task {
        friend ThreadPool;
        public:
            virtual void run() = 0;
            virtual void runFinal() final;
            virtual void wait() final;
        private:
            bool done = false;
            std::mutex m;
            std::condition_variable cv;
        };

        template<typename F>
        class TaskTemplate : public Task {
        private:
            const F &func;
        public:
            TaskTemplate(const F &f):func(f){}
            virtual void run(){
                func();
            }
        };
    private:
        void workerFunction();
        std::list<std::thread> threads;
        
        SharedQueue<Task*> tasks;
    public:
        ThreadPool(size_t n);
        void submit(Task *task);
    };
}
