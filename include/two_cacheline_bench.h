#pragma once

#include <cstdint>
#include <vector>

/**
 * @brief Two cache line latency benchmark
 */
namespace TwoCachelineBench {

std::vector<uint64_t> Measure(int core1_id, int core2_id, int num_inner_rounds);

} // namespace TwoCachelineBench

