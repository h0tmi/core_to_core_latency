#include "benchmark_engine.h"
#include "cpu_utils.h"
#include "statistics.h"
#include <fstream>
#include <iostream>
#include <iomanip>

BenchmarkEngine::BenchmarkEngine() {
    num_cores_ = CPUUtils::GetNumCores();
    std::cerr << "Detected " << num_cores_ << " CPU cores\n";
}

int BenchmarkEngine::GetNumCores() const {
    return num_cores_;
}

void BenchmarkEngine::RunBenchmark(
    const std::string& bench_name,
    std::function<std::vector<uint64_t>(int, int, int)> bench_func,
    int num_iterations,
    int num_samples
) {
    std::cerr << "\nRunning benchmark: " << bench_name << "\n";
    std::cerr << "Iterations per sample: " << num_iterations << "\n";
    std::cerr << "Number of samples: " << num_samples << "\n\n";

    std::vector<std::tuple<int, int, uint64_t, uint64_t, uint64_t>> results;

    int total_pairs = num_cores_ * (num_cores_ - 1);
    int completed = 0;

    for (int i = 0; i < num_cores_; ++i) {
        for (int j = 0; j < num_cores_; ++j) {
            if (i == j) continue;

            std::cerr << "Measuring cores (" << i << " -> " << j << ")... ";
            std::cerr.flush();

            auto measurements = bench_func(i, j, num_iterations);

            auto [median, p90, p95] = Statistics::CalculateAllStats(measurements);

            results.push_back({i, j, median, p90, p95});

            completed++;
            std::cerr << "Done [" << completed << "/" << total_pairs << "]\n";
        }
    }

    std::string filename = "results/" + bench_name + ".csv";
    WriteResults(filename, results);

    std::cerr << "\nResults written to: " << filename << "\n";
}

void BenchmarkEngine::WriteResults(
    const std::string& filename,
    const std::vector<std::tuple<int, int, uint64_t, uint64_t, uint64_t>>& results
) {
    std::ofstream out(filename, std::ios::trunc);
    if (!out) {
        throw std::runtime_error("Failed to open output file: " + filename);
    }

    out << "core1,core2,latency_ns_med,latency_ns_p90,latency_ns_p95\n";

    for (const auto& [core1, core2, median, p90, p95] : results) {
        out << core1 << "," << core2 << ","
            << median << "," << p90 << "," << p95 << "\n";
    }

    out.close();
}

