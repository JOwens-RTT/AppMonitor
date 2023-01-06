#include "MemoryMonitor.h"

#define MEM_FILE_LOC    "/proc/meminfo"
#define TIMEOUT_MS      1000

void MemoryMonitor::work()
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

unsigned long long MemoryMonitor::parseLine(std::string line)
{
    std::string buf = "";
    for(unsigned long i = 0; i < line.size(); i++)
    {
        char pos = line.at(i);
        if((pos >= '0' && pos <= '9') || pos == ' ')
        {
            // Is numerical or space
            buf.push_back(pos);
        }
    }
    char* pEnd;
    if(buf != "") return strtoull(buf.c_str(), &pEnd, 10);
    return 0;
}