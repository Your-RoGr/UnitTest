#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include "get_memory_usage.h"
#include "../Timer/timer.h"

class Profiler {
private:
    Timer T {};
    std::ofstream file;
    std::string filename;
    std::thread t_profiler;
    std::mutex mtx {};
    size_t milliseconds = 10;
    bool profiling {};

public:
    explicit Profiler(size_t _milliseconds = 10);
    ~Profiler();

private:
    void stop();
    void str_to_CSV(float time, size_t ram);
    void profiler_thread_function();
};

#endif //PROFILER_H
