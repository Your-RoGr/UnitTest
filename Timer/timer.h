#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start {};
public:
    Timer();
    // Returns the current lifetime of the Timer object
    float get_current_time();
};

#endif //TIMER_H
