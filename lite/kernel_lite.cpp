#include "takt/kernel.hpp"
#include "takt/logger.hpp"

namespace takt {

bool Kernel::boot() {
    TAKT_LOGI("KernelLite", "TAKT OS Lite boot");
    if (!scheduler_.initAll()) return false;
    running_ = true;
    return true;
}

void Kernel::run() {
    scheduler_.run();
}

void Kernel::requestShutdown() {
    scheduler_.shutdownAll();
    running_ = false;
}

void Kernel::printStatistics() const {
    TAKT_LOGI("KernelLite", "Takts: %llu",
              static_cast<unsigned long long>(scheduler_.stats().totalTakts));
}

KernelStats Kernel::collectStats() const {
    KernelStats stats{};
    stats.scheduler = scheduler_.stats();
    return stats;
}

} // namespace takt
