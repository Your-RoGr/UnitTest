#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <psapi.h>

size_t get_memory_usage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
}

#elif defined(__APPLE__)

#include <sys/sysctl.h>

size_t get_memory_usage() {
    int mib[2];
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    u_int namelen = 2;
    uint64_t physical_memory;
    size_t length = sizeof(uint64_t);

    if (sysctl(mib, namelen, &physical_memory, &length, NULL, 0) == 0) {
        return physical_memory;
    } else {
        return 0;
    }
}

#else

#include <sys/sysinfo.h>

size_t get_memory_usage() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return (size_t)info.totalram - (size_t)info.freeram;
    }
    return 0;
}

#endif