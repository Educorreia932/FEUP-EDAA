#include "ThreadPool.h"

#include <iostream>

using namespace std;

const std::chrono::milliseconds utils::ThreadPool::TIMEOUT = std::chrono::milliseconds(100);

void utils::ThreadPool::Task::wait(){
    std::unique_lock<std::mutex> lock(m);
    while(!done){
        cv.wait(lock);
    }
}

void utils::ThreadPool::Task::runFinal(){
    std::lock_guard<std::mutex> guard(m);
    if(!done){
        this->run();
        done = true;
        cv.notify_all();
    }
}

void utils::ThreadPool::workerFunction(){
    Task *task;
    for(;;){
        {
            unique_lock<mutex> lk(m);
            while(tasks.empty()){
                cv.wait_for(lk, TIMEOUT);
                if(exit) return;
            }
            task = tasks.front();
            tasks.pop();
        }
        task->runFinal();
    }
}

utils::ThreadPool::ThreadPool(size_t n){
    while(threads.size() < n){
        threads.emplace_back(&ThreadPool::workerFunction, this);
    }
}

utils::ThreadPool::~ThreadPool(){
    {
        lock_guard guard(m);
        exit = true;
    }
    cv.notify_all();
    for(thread &t: threads)
        t.join();
}

void utils::ThreadPool::submit(Task *task){
    lock_guard guard(m);
    tasks.push(task);
    cv.notify_all();
}
