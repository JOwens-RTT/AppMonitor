#ifndef _CPU_MONITOR_H_
#define _CPU_MONITOR_H_

#include "MonitorThread.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>

struct CpuCore
{
    std::string tag;
    unsigned long long time;
    double percent;
};

struct CpuIdle
{
    std::chrono::milliseconds timeStamp;
    unsigned long long totalIdle;
    std::vector<CpuCore> coreIdle;
    std::string toString()
    {
        std::stringstream cores;
        for(unsigned long i = 0; i < coreIdle.size(); i++)
        {
            cores << coreIdle.at(i).tag << ": " << coreIdle.at(i).time;
            if(i < coreIdle.size() - 1) cores << ", ";
        }
        std::stringstream ret;
        ret << "[ " << timeStamp.count() << "] Total: " << totalIdle << ", Cores [" << cores.str() << "]";
        return ret.str();
    }
};

struct CpuUsage
{
    double total;
    std::vector<CpuCore> core;
    std::chrono::milliseconds timeStamp;
    std::string toString()
    {
        std::stringstream cores;
        for(unsigned long i = 0; i < core.size(); i++)
        {
            cores << core.at(i).tag << ": " << core.at(i).percent * 100.0;
            if(i < core.size() - 1) cores << ", ";
        }
        std::stringstream ret;
        ret << "[ " << timeStamp.count() << "] Total: " << total * 100.0 << ", Cores [" << cores.str() << "]";
        return ret.str();
    }
};

class CpuMonitor : public MonitorThread<CpuUsage>
{
public:
    CpuMonitor() : MonitorThread<CpuUsage>(),
    startTime(std::chrono::high_resolution_clock::now())
    {}
protected:
    void work() override;
private:
    const std::chrono::high_resolution_clock::time_point startTime;
    unsigned long long parseLine(std::string line, std::string& tag);
};

#endif // _CPU_MONITOR_H_