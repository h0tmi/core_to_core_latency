#pragma once

#include <cstdint>

/**
 * @brief Time Stamp Counter (TSC) based high-resolution clock
 */
class TSCClock {
public:
    TSCClock();

    static inline uint64_t Start();

    static inline uint64_t End();

    inline uint64_t ToNanoseconds(uint64_t ticks) const {
        return static_cast<uint64_t>(ticks * ns_per_tick_);
    }

    inline double GetNsPerTick() const {
        return ns_per_tick_;
    }

private:
    double ns_per_tick_;

    double CalibrateToNanoseconds();
};

inline uint64_t TSCClock::Start() {
    unsigned junk;
    asm volatile("cpuid" : "=a"(junk) : "a"(0) : "rbx", "rcx", "rdx");

    uint64_t low, high;
    asm volatile("rdtsc" : "=a"(low), "=d"(high));
    return (high << 32) | low;
}

inline uint64_t TSCClock::End() {
    unsigned aux;
    uint64_t low, high;
    asm volatile("rdtscp" : "=a"(low), "=d"(high), "=c"(aux));

    asm volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
    return (high << 32) | low;
}

