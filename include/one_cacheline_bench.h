#pragma once

#include <cstdint>
#include <vector>

/**
 * @brief One cache line latency benchmark
 */
namespace OneCachelineBench {

std::vector<uint64_t> Measure(int core1_id, int core2_id, int num_inner_rounds);

} // namespace OneCachelineBench

