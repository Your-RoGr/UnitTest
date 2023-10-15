#include "profiler.h"

Profiler::Profiler(size_t _milliseconds) : profiling(true), filename("_profiler.csv"), file(filename), milliseconds(_milliseconds) {

    if (!file.is_open()) {
        file.open(filename);
    }
    file << "time,RAM\n";
    file.close();
    file.open(filename, std::ios_base::app);
    t_profiler = std::thread(&Profiler::profiler_thread_function, this);
}

Profiler::~Profiler() {
    stop();
    if (file.is_open()) {
        file.close();
    }
}

void Profiler::stop() {
    profiling = false;
    if (t_profiler.joinable()) t_profiler.join();
}

void Profiler::str_to_CSV(float time, size_t ram) {
    if (!file.is_open()) {
        file.open(filename, std::ios_base::app);
    }
    file << time << "," << ram << '\n';
}

void Profiler::profiler_thread_function() {
    while (profiling) {
        std::lock_guard<std::mutex> lock(mtx);
        str_to_CSV(T.get_current_time(), get_memory_usage());
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
}

