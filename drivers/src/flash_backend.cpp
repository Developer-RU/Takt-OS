#include "takt/drivers/flash_backend.hpp"
#include "takt/storage_manager.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "esp_partition.h"
#endif

namespace takt::drivers {

namespace {
constexpr const char* TAG = "FlashBackend";

#ifdef TAKT_ESP32
struct PartitionCtx {
    const esp_partition_t* part = nullptr;
};

int partitionRead(uint32_t offset, void* buf, size_t len, PartitionCtx* ctx) {
    if (!ctx || !ctx->part) return -1;
    if (offset + len > ctx->part->size) return -1;
    return esp_partition_read(ctx->part, offset, buf, len) == ESP_OK ? static_cast<int>(len) : -1;
}

int partitionWrite(uint32_t offset, const void* buf, size_t len, PartitionCtx* ctx) {
    if (!ctx || !ctx->part) return -1;
    if (offset + len > ctx->part->size) return -1;
    return esp_partition_write(ctx->part, offset, buf, len) == ESP_OK ? static_cast<int>(len) : -1;
}

int partitionErase(uint32_t offset, size_t len, PartitionCtx* ctx) {
    if (!ctx || !ctx->part) return -1;
    return esp_partition_erase_range(ctx->part, offset, len) == ESP_OK ? 0 : -1;
}

static PartitionCtx g_storageCtx{};
#endif
}

bool createPartitionBackend(FlashBackend& backend, const char* label) {
#ifdef TAKT_ESP32
    g_storageCtx.part = esp_partition_find_first(ESP_PARTITION_TYPE_ANY,
                                                 ESP_PARTITION_SUBTYPE_ANY, label);
    if (!g_storageCtx.part) {
        TAKT_LOGE(TAG, "Partition '%s' not found", label);
        return false;
    }
    backend.read = [ctx = &g_storageCtx](uint32_t offset, void* buf, size_t len) {
        return partitionRead(offset, buf, len, ctx);
    };
    backend.write = [ctx = &g_storageCtx](uint32_t offset, const void* buf, size_t len) {
        return partitionWrite(offset, buf, len, ctx);
    };
    backend.erase = [ctx = &g_storageCtx](uint32_t offset, size_t len) {
        return partitionErase(offset, len, ctx);
    };
    backend.totalSize = g_storageCtx.part->size;
    backend.sectorSize = 4096;
    TAKT_LOGI(TAG, "Partition '%s': size=%u", label, backend.totalSize);
    return true;
#else
    (void)backend; (void)label;
    return false;
#endif
}

bool createStorageBackend(FlashBackend& backend) {
    return createPartitionBackend(backend, "storage");
}

bool initStorageFromPartition(const char* label) {
    FlashBackend backend{};
    if (!createPartitionBackend(backend, label)) return false;
    return StorageManager::instance().init(backend);
}

} // namespace takt::drivers
