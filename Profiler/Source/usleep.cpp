#include "usleep.h"

#if defined(_WIN32) || defined(_WIN64)
void usleep(size_t usec) {
    LARGE_INTEGER starting_time, ending_time, elapsed_microseconds;
    LARGE_INTEGER frequency;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&starting_time);

    do {
        QueryPerformanceCounter(&ending_time);
        elapsed_microseconds.QuadPart = ending_time.QuadPart - starting_time.QuadPart;
        elapsed_microseconds.QuadPart *= 1000000;
        elapsed_microseconds.QuadPart /= frequency.QuadPart;
    } while (elapsed_microseconds.QuadPart < static_cast<LONGLONG>(usec));
}
#else
void usleep(size_t usec) {
        std::this_thread::sleep_for(std::chrono::microseconds(usec));
}
#endif
