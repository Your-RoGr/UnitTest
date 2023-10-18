#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include "Source/get_memory_usage.h"
#include "../Timer/timer.h"

class Profiler {
private:
    Timer T {};
    std::ofstream profiler_file;
    std::ofstream tests_file;
    std::string profiler_filename;
    std::string tests_filename;
    std::thread profiler_t;
    std::mutex profiler_mtx {};
    size_t microseconds = 10;
    bool profiling {};

public:

    explicit Profiler(size_t _microseconds = 100);
    void add_test(const std::string& test);
    void set_us(size_t _microseconds);
    ~Profiler();
private:
    void stop();

    template<typename T>
    void str_to_CSV(std::ofstream& file, const std::string& filename, float time, T data) {
        if (!file.is_open()) {
            file.open(filename, std::ios_base::app);
        }
        file << time << "," << data << '\n';
        file.flush();
    }

    void profiler_thread_function();
};

#endif //PROFILER_H
