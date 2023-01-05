#ifndef _MONITOR_THREAD_H_
#define _MONITOR_THREAD_H_

#include <thread>
#include <SafeQueue.h>

template <class T>
class MonitorThread
{
public:
    MonitorThread();
    T read();
    void end();
protected:
    virtual void work() = 0;
    SafeQueue<T> q;
private:
    std::thread worker;
    void workerProcess();
    volatile bool exitFlag = false;
};

template <class T>
MonitorThread<T>::MonitorThread() : worker(&MonitorThread::workerProcess, this)
{
}

template <class T>
T MonitorThread<T>::read()
{
    return q.dequeue();
}

template <class T>
void MonitorThread<T>::end()
{
    exitFlag = true;
}

template <class T>
void MonitorThread<T>::workerProcess()
{
    while(!exitFlag) work();
}



#endif // _MONITOR_THREAD_H_