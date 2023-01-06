#ifndef _MEMORY_MONITOR_H_
#define _MEMORY_MONITOR_H_

#include "MonitorThread.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>

struct MemoryUsage
{
    std::chrono::milliseconds timeStamp;
    unsigned long long total;
    unsigned long long free;
    unsigned long long swapTotal;
    unsigned long long swapFree;
    std::string toString()
    {
        std::stringstream ret;
        ret << "[ " << timeStamp.count() << " ] Total: " << total << ", Free: " << free << ", Total Swap: " << swapTotal << ", Free Swap: " << swapFree;
        return ret.str();
    }
};

class MemoryMonitor : public MonitorThread<MemoryUsage>
{
public:
    MemoryMonitor() : MonitorThread<MemoryUsage>(),
    startTime(std::chrono::high_resolution_clock::now())
    {
    }
protected:
    void work() override;
private:
    const std::chrono::high_resolution_clock::time_point startTime;
    unsigned long long parseLine(std::string line);
};

#endif // _MEMORY_MONITOR_H_