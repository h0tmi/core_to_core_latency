#include "tsc_clock.h"

#include <time.h>
#include <iostream>

TSCClock::TSCClock() {
    ns_per_tick_ = CalibrateToNanoseconds();
    std::cerr << "TSC calibration: " << ns_per_tick_ << " ns/tick\n";
}

double TSCClock::CalibrateToNanoseconds() {
    const int num_measurements = 100;
    const int sleep_ms = 10;
    double cumsum = 0.0;

    for (int i = 0; i < num_measurements; ++i) {
        uint64_t tsc_start = Start();
        struct timespec time_start;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_start);

        struct timespec sleep_duration = {0, sleep_ms * 1000000};
        nanosleep(&sleep_duration, nullptr);

        uint64_t tsc_end = End();
        struct timespec time_end;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_end);

        uint64_t delta_ns = (time_end.tv_sec - time_start.tv_sec) * 1000000000ULL +
                           (time_end.tv_nsec - time_start.tv_nsec);
        uint64_t delta_tsc = tsc_end - tsc_start;

        cumsum += static_cast<double>(delta_ns) / static_cast<double>(delta_tsc);
    }

    return cumsum / num_measurements;
}

