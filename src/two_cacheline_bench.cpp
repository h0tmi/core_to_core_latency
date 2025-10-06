#include "two_cacheline_bench.h"
#include "tsc_clock.h"
#include "cpu_utils.h"

#include <atomic>
#include <thread>
#include <iostream>

namespace TwoCachelineBench {

alignas(64) static std::atomic<int> signal1{-1};
alignas(64) static std::atomic<int> signal2{-1};

static TSCClock tsc_clock;

static void WorkerThread(int cpu_id, int num_samples, int num_inner_rounds) {
    CPUUtils::PinThreadToCore(cpu_id);

    CPUUtils::WarmUp(200000000ULL);

    for (int sample = 0; sample < num_samples; ++sample) {
        for (int round = 0; round < num_inner_rounds; ++round) {
            while (signal1.load(std::memory_order_acquire) != round) {
                // Busy wait
            }

            signal2.store(round, std::memory_order_release);
        }
    }
}

std::vector<uint64_t> Measure(int core1_id, int core2_id, int num_inner_rounds) {
    const int num_samples = 1000;
    std::vector<uint64_t> measurements;
    measurements.reserve(num_samples);

    std::thread worker(WorkerThread, core2_id, num_samples, num_inner_rounds);

    CPUUtils::PinThreadToCore(core1_id);

    CPUUtils::WarmUp(200000000ULL);

    for (int sample = 0; sample < num_samples; ++sample) {
        signal1.store(-1, std::memory_order_release);
        signal2.store(-1, std::memory_order_release);

        for (int i = 0; i < 1000; ++i) {
            asm volatile("" ::: "memory");
        }

        uint64_t start_tsc = TSCClock::Start();

        for (int round = 0; round < num_inner_rounds; ++round) {
            signal1.store(round, std::memory_order_release);

            while (signal2.load(std::memory_order_acquire) != round) {
                // Busy wait
            }
        }

        uint64_t end_tsc = TSCClock::End();

        uint64_t latency_ns = tsc_clock.ToNanoseconds(end_tsc - start_tsc) / (2 * num_inner_rounds);
        measurements.push_back(latency_ns);
    }

    worker.join();

    return measurements;
}

} // namespace TwoCachelineBench

