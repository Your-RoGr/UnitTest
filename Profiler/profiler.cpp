#include "profiler.h"
#include "Source/usleep.h"


Profiler::Profiler(size_t _microseconds) : profiling(true), profiler_filename(".profiler.csv"),
profiler_file(profiler_filename), tests_filename(".tests.csv"), tests_file(tests_filename),
microseconds(_microseconds) {

    if (!profiler_file.is_open()) {
        profiler_file.open(profiler_filename);
    }

    profiler_file << "time,RAM\n";
    profiler_file.close();
    profiler_file.open(profiler_filename, std::ios_base::app);

    if (!tests_file.is_open()) {
        tests_file.open(tests_filename);
    }

    tests_file << "time,labels\n";
    tests_file.close();
    tests_file.open(tests_filename, std::ios_base::app);

    profiler_t = std::thread(&Profiler::profiler_thread_function, this);
}

Profiler::~Profiler() {

    stop();

    if (profiler_file.is_open()) {
        profiler_file.close();
    }

    if (tests_file.is_open()) {
        tests_file.close();
    }

    try {
        std::remove(profiler_filename.c_str());
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "File deletion error: " << profiler_filename << std::endl;
    }

    try {
        std::remove(tests_filename.c_str());
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "File deletion error: " << tests_filename << std::endl;
    }
}

void Profiler::stop() {
    profiling = false;
    if (profiler_t.joinable()) profiler_t.join();
}

void Profiler::profiler_thread_function() {
    while (profiling) {
        std::lock_guard<std::mutex> lock(profiler_mtx);
        usleep(microseconds);
        str_to_CSV(profiler_file, profiler_filename, T.get_current_time(), get_memory_usage());
    }
}

void Profiler::add_test(const std::string& test) {
    str_to_CSV(tests_file, tests_filename, T.get_current_time(), test);
}


