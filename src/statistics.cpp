#include "statistics.h"

#include <algorithm>
#include <stdexcept>

namespace Statistics {

uint64_t CalculateMedian(const std::vector<uint64_t>& measurements) {
    if (measurements.empty()) {
        throw std::runtime_error("Cannot calculate median of empty vector");
    }
    size_t n = measurements.size();
    return measurements[n / 2];
}

uint64_t CalculateP90(const std::vector<uint64_t>& measurements) {
    if (measurements.empty()) {
        throw std::runtime_error("Cannot calculate P90 of empty vector");
    }
    size_t n = measurements.size();
    size_t idx = static_cast<size_t>(0.90 * n);
    if (idx >= n) idx = n - 1;
    return measurements[idx];
}

uint64_t CalculateP95(const std::vector<uint64_t>& measurements) {
    if (measurements.empty()) {
        throw std::runtime_error("Cannot calculate P95 of empty vector");
    }
    size_t n = measurements.size();
    size_t idx = static_cast<size_t>(0.95 * n);
    if (idx >= n) idx = n - 1;
    return measurements[idx];
}

uint64_t CalculateP99(const std::vector<uint64_t>& measurements) {
    if (measurements.empty()) {
        throw std::runtime_error("Cannot calculate P99 of empty vector");
    }
    size_t n = measurements.size();
    size_t idx = static_cast<size_t>(0.99 * n);
    if (idx >= n) idx = n - 1;
    return measurements[idx];
}

std::tuple<uint64_t, uint64_t, uint64_t> CalculateAllStats(std::vector<uint64_t>& measurements) {
    if (measurements.empty()) {
        throw std::runtime_error("Cannot calculate statistics of empty vector");
    }

    std::sort(measurements.begin(), measurements.end());

    return {
        CalculateMedian(measurements),
        CalculateP90(measurements),
        CalculateP95(measurements)
    };
}

} // namespace Statistics

