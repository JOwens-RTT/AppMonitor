#ifndef _APP_MONITOR_H_
#define _APP_MONITOR_H_

#include "MemoryMonitor.h"

struct AppStats
{
    MemoryUsage memory;
};

class AppMonitor
{
public:
    AppMonitor() {}
    AppStats read()
    {
        stats.memory = memory.read();
        return stats;
    }
private:
    AppStats stats;
    MemoryMonitor memory;
};

#endif // _APP_MONITOR_H_