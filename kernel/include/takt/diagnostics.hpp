#pragma once

#include <cstddef>
#include <cstdint>

namespace takt {

/// Memory region descriptor for flash-backed storage.
struct FlashRegion {
    uint32_t offset;   ///< Byte offset from flash base
    uint32_t size;     ///< Region size in bytes
    const char* label; ///< Human-readable label
};

/// Heap memory snapshot.
struct HeapInfo {
    size_t totalFree     = 0;
    size_t minFreeEver   = 0;
    size_t largestBlock  = 0;
    size_t allocated     = 0;
};

/// Stack usage snapshot for a task or module.
struct StackInfo {
    const char* name       = nullptr;
    size_t      sizeBytes  = 0;
    size_t      usedBytes  = 0;
    size_t      freeBytes  = 0;
    float       usagePct   = 0.0f;
};

/// Diagnostics and profiling subsystem.
class Diagnostics {
public:
    static Diagnostics& instance() {
        static Diagnostics diag;
        return diag;
    }

    /// Sample current heap state.
    HeapInfo heapInfo() const;

    /// Register a stack region for monitoring.
    void registerStack(const char* name, void* stackTop, size_t sizeBytes);

    /// Check all registered stacks for overflow (canary pattern).
    bool checkStacks();

    /// Print profiling report.
    void printReport() const;

    /// Record a profiling sample.
    void recordSample(const char* label, uint64_t durationUs);

private:
    static constexpr size_t kMaxStacks = 16;
    struct StackEntry {
        const char* name = nullptr;
        void*       top  = nullptr;
        size_t      size = 0;
    };
    StackEntry stacks_[kMaxStacks]{};
    size_t     stackCount_ = 0;
};

} // namespace takt
