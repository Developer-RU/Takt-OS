#include "takt/diagnostics.hpp"
#include "takt/logger.hpp"
#include <cstdio>
#include <cstring>

#ifdef TAKT_ESP32
#include "esp_heap_caps.h"
#endif

namespace takt {

namespace {
constexpr const char* TAG = "Diagnostics";
constexpr uint32_t STACK_CANARY = 0xDEADBEEF;
}

HeapInfo Diagnostics::heapInfo() const {
    HeapInfo info{};
#ifdef TAKT_ESP32
    info.totalFree    = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    info.minFreeEver  = heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);
    info.largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
    info.allocated    = heap_caps_get_total_size(MALLOC_CAP_DEFAULT) - info.totalFree;
#else
    info.totalFree    = 256 * 1024;
    info.minFreeEver  = 200 * 1024;
    info.largestBlock = 128 * 1024;
    info.allocated    = 56 * 1024;
#endif
    return info;
}

void Diagnostics::registerStack(const char* name, void* stackTop, size_t sizeBytes) {
    if (stackCount_ >= kMaxStacks) return;
    stacks_[stackCount_] = {name, stackTop, sizeBytes};
    ++stackCount_;
}

bool Diagnostics::checkStacks() {
    bool ok = true;
    for (size_t i = 0; i < stackCount_; ++i) {
        auto& s = stacks_[i];
        if (!s.top) continue;
        auto* canary = static_cast<uint32_t*>(s.top);
        if (*canary != STACK_CANARY) {
            TAKT_LOGE(TAG, "Stack overflow in '%s'", s.name ? s.name : "?");
            ok = false;
        }
    }
    return ok;
}

void Diagnostics::recordSample(const char* label, uint64_t durationUs) {
    TAKT_LOGD(TAG, "Profile [%s]: %llu us",
              label, static_cast<unsigned long long>(durationUs));
}

void Diagnostics::printReport() const {
    auto heap = heapInfo();
    TAKT_LOGI(TAG, "=== Diagnostics Report ===");
    TAKT_LOGI(TAG, "Heap: free=%u min=%u largest=%u allocated=%u",
              static_cast<unsigned>(heap.totalFree),
              static_cast<unsigned>(heap.minFreeEver),
              static_cast<unsigned>(heap.largestBlock),
              static_cast<unsigned>(heap.allocated));
    TAKT_LOGI(TAG, "Monitored stacks: %u", static_cast<unsigned>(stackCount_));
    TAKT_LOGI(TAG, "=== End Report ===");
}

} // namespace takt
