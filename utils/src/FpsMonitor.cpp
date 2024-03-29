#include "FpsMonitor.h"

FpsMonitor::FpsMonitor(int ms):
    Dt(std::chrono::milliseconds(ms))
{
}

void FpsMonitor::setInterval(int ms){
    Dt = std::chrono::milliseconds(ms);
}

void FpsMonitor::count(){
    auto now = std::chrono::high_resolution_clock::now();
    q.push(now);
    while(now-q.front() > Dt){
        q.pop();
    }
}

float FpsMonitor::getFPS() const{
    return float(q.size())/(float(std::chrono::duration_cast<std::chrono::milliseconds>(Dt).count())/1000.0f);
}
