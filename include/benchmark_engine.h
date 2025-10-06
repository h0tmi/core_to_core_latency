#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <functional>

/**
 * @brief Main benchmark engine that coordinates latency measurements
 */
class BenchmarkEngine {
public:
    BenchmarkEngine();

    int GetNumCores() const;

    void RunBenchmark(
        const std::string& bench_name,
        std::function<std::vector<uint64_t>(int, int, int)> bench_func,
        int num_iterations,
        int num_samples
    );

private:
    void WriteResults(
        const std::string& filename,
        const std::vector<std::tuple<int, int, uint64_t, uint64_t, uint64_t>>& results
    );

private:
    int num_cores_;
};

