#include <iostream>
#include <string>
#include <sstream>

#include <AppMonitor.h>

AppMonitor monitor;

int main()
{
    while(true)
    {
        AppStats stats = monitor.read();
        std::cout << stats.memory.toString() << std::endl << stats.cpu.toString() << std::endl;
    }
    return 0;
}