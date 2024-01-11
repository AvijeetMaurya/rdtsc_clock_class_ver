#include "rdtsc_clock.hpp"

#include <x86intrin.h>
#include <cpuid.h>

double RDTSC_Clock::tsc_time_period{};
unsigned long long RDTSC_Clock::time_since_boot{};
unsigned int RDTSC_Clock::cpu_id{};
std::jthread RDTSC_Clock::worker{};

void RDTSC_Clock::init() {
    tsc_time_period = get_tsc_period();
    time_since_boot = std::chrono::system_clock::now().time_since_epoch().count() - (__rdtscp(&cpu_id) * tsc_time_period);
    worker = std::jthread(sync, std::ref(time_since_boot));
}

unsigned long long RDTSC_Clock::now() {
    return time_since_boot + static_cast<unsigned long long>(__rdtscp(&cpu_id) * tsc_time_period);
}

void RDTSC_Clock::exit() {
    worker.request_stop();
    worker.detach();
}

void RDTSC_Clock::sync(std::stop_token status, unsigned long long& time_since_boot) {
    while (!status.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        time_since_boot = std::chrono::system_clock::now().time_since_epoch().count() - (__rdtscp(&cpu_id) * tsc_time_period);
    }
}

double RDTSC_Clock::get_tsc_period() {
    unsigned int eax_denominator, ebx_numerator, ecx_hz, edx;
    __get_cpuid(0x15, &eax_denominator, &ebx_numerator, &ecx_hz, &edx);
    return (eax_denominator * 1e9) / (static_cast<double>(ecx_hz) * ebx_numerator);
}
