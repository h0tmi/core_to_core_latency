#include "cpu_utils.h"
#include "tsc_clock.h"
#include <sched.h>
#include <unistd.h>
#include <pthread.h>
#include <stdexcept>

namespace CPUUtils {

void PinThreadToCore(int cpu_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);

    int result = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (result != 0) {
        throw std::runtime_error("Failed to set thread affinity to CPU " + std::to_string(cpu_id));
    }
}

int GetNumCores() {
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cores <= 0) {
        throw std::runtime_error("Failed to get number of CPU cores");
    }
    return static_cast<int>(num_cores);
}

void WarmUp(uint64_t duration_cycles) {
    uint64_t start = TSCClock::Start();
    while (true) {
        uint64_t now = TSCClock::End();
        if ((now - start) >= duration_cycles) {
            break;
        }
    }
}

} // namespace CPUUtils

