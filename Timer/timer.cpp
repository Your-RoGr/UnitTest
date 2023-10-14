#include "timer.h"

Timer::Timer() {
    start = std::chrono::high_resolution_clock::now();
}

float Timer::get_current_time() {
    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - start;
    return duration.count();
}
