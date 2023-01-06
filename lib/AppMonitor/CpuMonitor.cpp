#include "CpuMonitor.h"

#define CPU_FILE_LOC    "/proc/stat"
#define TIMEOUT_MS      1000

void CpuMonitor::work()
{
    // Get current system time and determine if ready for another reading
    static std::chrono::high_resolution_clock::time_point lastStamp = startTime;
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds delay = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastStamp);
    if(delay.count() < TIMEOUT_MS) return;
    lastStamp = now;

    static CpuIdle lastUsage;

    // Read cpu file
    std::string line;
    std::ifstream cpuFile(CPU_FILE_LOC);
    if(cpuFile.is_open())
    {
        // Read current usage
        CpuIdle cpuUsage;
        cpuUsage.timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        while(std::getline(cpuFile,line))
        {
            std::string tag = "";
            CpuCore core;
            if(line.find("cpu") != std::string::npos)
            {
                core.time = parseLine(line, tag);
                core.tag = tag;
            }

            if(tag == "cpu")
            {
                cpuUsage.totalIdle = core.time;
            }
            else if(tag != "")
            {
                cpuUsage.coreIdle.push_back(core);
            }
        }
        cpuFile.close();

        // Compare with last usage
        CpuUsage usageDiff;
        usageDiff.timeStamp = cpuUsage.timeStamp;
        double totalIdle = (1.0 * (cpuUsage.totalIdle - lastUsage.totalIdle)) / 100.0;
        double totalPercent = 1.0 - totalIdle * 1000.0 / (delay.count() * cpuUsage.coreIdle.size());
        usageDiff.total = totalPercent;

        for(unsigned long i = 0; i < lastUsage.coreIdle.size(); i++)
        {
            CpuCore core;
            core.tag = cpuUsage.coreIdle.at(i).tag;
            double coreIdle = (1.0 * (cpuUsage.coreIdle.at(i).time - lastUsage.coreIdle.at(i).time)) / 100.0;
            double corePercent = 1.0 - coreIdle * 1000.0 / (delay.count());
            core.percent = corePercent;
            usageDiff.core.push_back(core);
        }
        if(lastUsage.coreIdle.size() > 0) q.enqueue(usageDiff);
        lastUsage = cpuUsage;
    }
}

unsigned long long CpuMonitor::parseLine(std::string line, std::string& tag)
{
    std::string buf = "";
    bool firstSpace = false;
    for(unsigned long i = 0; i < line.size(); i++)
    {
        char pos = line.at(i);
        if(pos != ' ' && pos != ':' && !firstSpace)
        {
            tag.push_back(pos);
        }
        else if(pos == ' ' && !firstSpace)
        {
            firstSpace = true;
        }
        else if((pos >= '0' && pos <= '9') || pos == ' ')
        {
            buf.push_back(pos);
        }
    }
    char* pEnd;
    if(buf != "") 
    {
        unsigned long long res = 0;
        pEnd = const_cast<char*>(buf.c_str());
        // Need the 4th number
        for(int i = 0; i < 4; i++)
        {
            res = strtoull(pEnd, &pEnd, 10);
            //std::cout << "Parse #" << i << ": " << res << std::endl;
        } 
        return res;
    }

    return 0;
}