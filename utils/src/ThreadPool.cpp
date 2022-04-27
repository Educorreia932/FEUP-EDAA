#include "ThreadPool.h"

using namespace std;

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
    for(;;){
        Task *task = tasks.popWait();
        task->runFinal();
    }
}

utils::ThreadPool::ThreadPool(size_t n){
    while(threads.size() < n){
        threads.emplace_back(&ThreadPool::workerFunction, this);
    }
}

void utils::ThreadPool::submit(Task *task){
    tasks.push(task);
}
