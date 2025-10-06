#include "benchmark_engine.h"
#include "one_cacheline_bench.h"
#include "two_cacheline_bench.h"

#include <iostream>
#include <string>
#include <cstring>

void PrintUsage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " [OPTIONS]\n\n";
    std::cerr << "Options:\n";
    std::cerr << "  --iterations N    Number of ping-pong rounds per sample (default: 100)\n";
    std::cerr << "  --both           Run both one and two cacheline benchmarks (default)\n";
    std::cerr << "  --one            Run only one cacheline benchmark\n";
    std::cerr << "  --two            Run only two cacheline benchmark\n";
    std::cerr << "  --help           Show this help message\n\n";
    std::cerr << "Examples:\n";
    std::cerr << "  " << program_name << "                    # Run both benchmarks with default settings\n";
    std::cerr << "  " << program_name << " --iterations 200   # Use 200 iterations per sample\n";
    std::cerr << "  " << program_name << " --one              # Run only one cacheline benchmark\n";
}

int main(int argc, char* argv[]) {
    int num_iterations = 100;
    bool run_one = false;
    bool run_two = false;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--iterations") == 0) {
            if (i + 1 < argc) {
                num_iterations = std::atoi(argv[++i]);
                if (num_iterations <= 0) {
                    std::cerr << "Error: iterations must be positive\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: --iterations requires an argument\n";
                return 1;
            }
        } else if (std::strcmp(argv[i], "--one") == 0) {
            run_one = true;
        } else if (std::strcmp(argv[i], "--two") == 0) {
            run_two = true;
        } else if (std::strcmp(argv[i], "--both") == 0) {
            run_one = true;
            run_two = true;
        } else if (std::strcmp(argv[i], "--help") == 0) {
            PrintUsage(argv[0]);
            return 0;
        } else {
            std::cerr << "Error: Unknown option " << argv[i] << "\n\n";
            PrintUsage(argv[0]);
            return 1;
        }
    }

    if (!run_one && !run_two) {
        run_one = true;
        run_two = true;
    }

    std::cerr << "======================================\n";
    std::cerr << "  Inter-Core Latency Benchmark\n";
    std::cerr << "======================================\n";

    try {
        BenchmarkEngine engine;

        if (run_one) {
            engine.RunBenchmark(
                "latencies_one_cacheline",
                [](int core1, int core2, int iters) {
                    return OneCachelineBench::Measure(core1, core2, iters);
                },
                num_iterations,
                1000
            );
        }

        if (run_two) {
            engine.RunBenchmark(
                "latencies_two_cacheline",
                [](int core1, int core2, int iters) {
                    return TwoCachelineBench::Measure(core1, core2, iters);
                },
                num_iterations,
                1000
            );
        }

        std::cerr << "\n======================================\n";
        std::cerr << "  Benchmark completed successfully!\n";
        std::cerr << "======================================\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

