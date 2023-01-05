#include <iostream>
#include <string>
#include <sstream>

#include <AppMonitor.h>

std::string appStatsToString(AppStats stats)
{
    std::stringstream ret;
    ret << "[ " << stats.memory.timeStamp.count() << " ] Total: " << stats.memory.total << ", Free: " << stats.memory.free << ", Total Swap: " << stats.memory.swapTotal << ", Free Swap: " << stats.memory.swapFree << std::endl;
    return ret.str();
}

AppMonitor monitor;

int main()
{
    while(true)
    {
        std::cout << appStatsToString(monitor.read()) << std::endl;
    }
    return 0;
}