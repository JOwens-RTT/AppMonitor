#ifndef _MEMORY_MONITOR_H_
#define _MEMORY_MONITOR_H_

#include "MonitorThread.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#define MEM_FILE_LOC "/proc/meminfo"
#define TIMEOUT_MS  1000

struct MemoryUsage
{
    std::chrono::milliseconds timeStamp;
    unsigned long long total;
    unsigned long long free;
    unsigned long long swapTotal;
    unsigned long long swapFree;
};

class MemoryMonitor : public MonitorThread<MemoryUsage>
{
public:
    MemoryMonitor() : MonitorThread<MemoryUsage>(),
    startTime(std::chrono::high_resolution_clock::now())
    {
    }
protected:
    void work() override
    {
        // Get current system time and determine if ready for another reading
        static std::chrono::high_resolution_clock::time_point lastStamp = startTime;
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds delay = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastStamp);
        if(delay.count() < TIMEOUT_MS) return;
        lastStamp = now;

        // Read memory file
        std::string line;
        std::ifstream memFile(MEM_FILE_LOC);
        if(memFile.is_open())
        {
            MemoryUsage memUsage;
            memUsage.timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
            while(std::getline(memFile,line))
            {
                if(line.find("MemTotal:") != std::string::npos)
                {
                    memUsage.total = parseLine(line);
                }
                else if(line.find("MemFree:") != std::string::npos)
                {
                    memUsage.free = parseLine(line);
                }
                else if(line.find("SwapTotal:") != std::string::npos)
                {
                    memUsage.swapTotal = parseLine(line);
                }
                else if(line.find("SwapFree:") != std::string::npos)
                {
                    memUsage.swapFree = parseLine(line);
                }
            }
            memFile.close();
            q.enqueue(memUsage);
        }
    }
private:
    const std::chrono::high_resolution_clock::time_point startTime;
    unsigned long long parseLine(std::string line)
    {
        std::string buf = "";
        for(unsigned long i = 0; i < line.size(); i++)
        {
            char pos = line.at(i);
            if(pos >= '0' && pos <= '9')
            {
                // Is numerical
                buf.push_back(pos);
            }
        }
        char* pEnd;
        if(buf != "") return strtoull(buf.c_str(), &pEnd, 10);
        return 0;
    }
};

#endif // _MEMORY_MONITOR_H_