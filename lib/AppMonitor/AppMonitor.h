#ifndef _APP_MONITOR_H_
#define _APP_MONITOR_H_

#include "MemoryMonitor.h"
#include "CpuMonitor.h"

struct AppStats
{
    MemoryUsage memory;
    CpuUsage cpu;
};

class AppMonitor
{
public:
    AppMonitor() {}
    AppStats read()
    {
        stats.memory = memory.read();
        stats.cpu = cpu.read();
        return stats;
    }
private:
    AppStats stats;
    MemoryMonitor memory;
    CpuMonitor cpu;
};

#endif // _APP_MONITOR_H_