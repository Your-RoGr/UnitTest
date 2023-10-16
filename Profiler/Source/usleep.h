#ifndef USLEEP_H
#define USLEEP_H

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <chrono>
#include <thread>
#endif

void usleep(size_t usec);

#endif // USLEEP_H
