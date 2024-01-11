#pragma once

#include <chrono>
#include <thread>

class RDTSC_Clock {
public:
    static void init();
    static unsigned long long now();
    static void exit();

private:
    RDTSC_Clock();
    static double get_tsc_period();
    static void sync(std::stop_token status, unsigned long long& time_since_boot);

    static double tsc_time_period;
    static unsigned long long time_since_boot;
    static unsigned int cpu_id;
    static std::jthread worker;
};