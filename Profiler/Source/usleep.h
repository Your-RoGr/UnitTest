#ifndef USLEEP_H
#define USLEEP_H

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <chrono>
#include <thread>
#endif

// Stops the current thread for the transmitted number of microseconds
// Needed to work on Windows
void usleep(size_t usec);

#endif // USLEEP_H
