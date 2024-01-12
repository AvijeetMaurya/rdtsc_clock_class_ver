#include <iostream>

#include "rdtsc_clock.hpp"

#include <time.h>

int main() {
    //RDTSC_Clock::init();
    timespec ts;
    int count = 10000000;
    while (count--) {  
        //auto now = std::chrono::system_clock::now();
        //long long rdtscp_timestamp = RDTSC_Clock::now();
        clock_gettime(CLOCK_REALTIME, &ts);
        volatile long long system_timestamp = static_cast<long long>(ts.tv_sec*1e9) + ts.tv_nsec;
        //std::cout << "Delta: " << rdtscp_timestamp - system_timestamp << '\n';
    }
    //RDTSC_Clock::exit();
}