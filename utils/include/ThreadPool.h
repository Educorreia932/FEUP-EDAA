#pragma once

#include <chrono>
#include <condition_variable>
#include <list>
#include <mutex>
#include <queue>
#include <thread>

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

        std::queue<Task*> tasks;
        std::mutex m;
        std::condition_variable cv;

        static const std::chrono::milliseconds TIMEOUT;
        bool exit = false;
    public:
        ThreadPool(size_t n);
        ~ThreadPool();
        void submit(Task *task);
    };
}
