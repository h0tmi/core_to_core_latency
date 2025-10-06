#pragma once

#include <cstdint>

/**
 * @brief CPU affinity and utility functions
 */
namespace CPUUtils {

void PinThreadToCore(int cpu_id);

int GetNumCores();

void WarmUp(uint64_t duration_cycles);

} // namespace CPUUtils

