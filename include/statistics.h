#pragma once

#include <cstdint>
#include <vector>

/**
 * @brief Statistical analysis utilities for latency measurements
 */
namespace Statistics {

uint64_t CalculateMedian(const std::vector<uint64_t>& measurements);

uint64_t CalculateP90(const std::vector<uint64_t>& measurements);

uint64_t CalculateP95(const std::vector<uint64_t>& measurements);

uint64_t CalculateP99(const std::vector<uint64_t>& measurements);

std::tuple<uint64_t, uint64_t, uint64_t> CalculateAllStats(std::vector<uint64_t>& measurements);

} // namespace Statistics

