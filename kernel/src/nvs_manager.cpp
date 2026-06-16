// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/nvs_manager.hpp"
#include "takt/crc32.hpp"
#include "takt/logger.hpp"
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <unordered_map>

#ifdef TAKT_ESP32
#include "nvs_flash.h"
#include "nvs.h"
#else
namespace {
struct HostNvsEntry {
    std::vector<uint8_t> data;
    uint16_t version = 0;
    uint32_t crc32 = 0;
};
std::unordered_map<std::string, HostNvsEntry> gHostNvs;
} // namespace
#endif

namespace takt {

namespace {
constexpr const char* TAG = "NVS";
constexpr const char* kBackupNs = "takt_bak";
constexpr const char* kBackupMagicKey = "_bak_magic";
constexpr uint32_t kBackupMagic = 0xBAAC0DE1;
}

bool NvsManager::init(const char* namespaceName) {
    std::strncpy(namespace_, namespaceName, sizeof(namespace_) - 1);

#ifdef TAKT_ESP32
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        TAKT_LOGE(TAG, "NVS flash init failed: %d", err);
        return false;
    }
    if (!verifyIntegrity()) {
        TAKT_LOGW(TAG, "Integrity check failed, restoring backup");
        restoreFromBackup();
    }
#endif

    initialized_ = true;
    TAKT_LOGI(TAG, "NVS initialized, namespace='%s'", namespace_);
    return true;
}

uint32_t NvsManager::computeCrc(const void* data, size_t len) const {
    return Crc32::compute(data, len);
}

bool NvsManager::writeEntry(const char* key, const void* data, size_t len, uint16_t version) {
#ifdef TAKT_ESP32
    nvs_handle_t handle;
    if (nvs_open(namespace_, NVS_READWRITE, &handle) != ESP_OK) return false;

    NvsEntry meta{};
    std::strncpy(meta.key, key, sizeof(meta.key) - 1);
    meta.version = version;
    meta.dataSize = static_cast<uint16_t>(len);
    meta.crc32 = computeCrc(data, len);

    char metaKey[40];
    snprintf(metaKey, sizeof(metaKey), "%s_meta", key);
    nvs_set_blob(handle, metaKey, &meta, sizeof(meta));
    nvs_set_blob(handle, key, data, len);
    const esp_err_t err = nvs_commit(handle);
    nvs_close(handle);
    return err == ESP_OK;
#else
    HostNvsEntry entry;
    entry.data.assign(static_cast<const uint8_t*>(data),
                      static_cast<const uint8_t*>(data) + len);
    entry.version = version;
    entry.crc32 = computeCrc(data, len);
    gHostNvs[std::string(namespace_) + ":" + key] = std::move(entry);
    return true;
#endif
}

bool NvsManager::readEntry(const char* key, void* data, size_t maxLen, uint16_t* version) const {
#ifdef TAKT_ESP32
    nvs_handle_t handle;
    if (nvs_open(namespace_, NVS_READONLY, &handle) != ESP_OK) return false;

    NvsEntry meta{};
    char metaKey[40];
    snprintf(metaKey, sizeof(metaKey), "%s_meta", key);
    size_t metaLen = sizeof(meta);
    if (nvs_get_blob(handle, metaKey, &meta, &metaLen) != ESP_OK) {
        nvs_close(handle);
        return false;
    }

    size_t dataLen = maxLen;
    if (nvs_get_blob(handle, key, data, &dataLen) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    nvs_close(handle);

    if (computeCrc(data, dataLen) != meta.crc32) {
        TAKT_LOGW(TAG, "CRC mismatch for '%s'", key);
        return false;
    }
    if (version) *version = meta.version;
    return true;
#else
    const auto it = gHostNvs.find(std::string(namespace_) + ":" + key);
    if (it == gHostNvs.end() || it->second.data.size() > maxLen) return false;
    std::memcpy(data, it->second.data.data(), it->second.data.size());
    if (computeCrc(data, it->second.data.size()) != it->second.crc32) return false;
    if (version) *version = it->second.version;
    return true;
#endif
}

bool NvsManager::setBlob(const char* key, const void* data, size_t len, uint16_t version) {
    return writeEntry(key, data, len, version);
}

int NvsManager::getBlob(const char* key, void* data, size_t maxLen, uint16_t* version) const {
#ifdef TAKT_ESP32
    nvs_handle_t handle;
    if (nvs_open(namespace_, NVS_READONLY, &handle) != ESP_OK) return -1;
    size_t dataLen = 0;
    if (nvs_get_blob(handle, key, nullptr, &dataLen) != ESP_OK) {
        nvs_close(handle);
        return -1;
    }
    nvs_close(handle);
    if (dataLen > maxLen) return -1;
    if (!readEntry(key, data, maxLen, version)) return -1;
    return static_cast<int>(dataLen);
#else
    (void)key; (void)data; (void)maxLen; (void)version;
    return -1;
#endif
}

bool NvsManager::setU8(const char* key, uint8_t value, uint16_t version) {
    return writeEntry(key, &value, sizeof(value), version);
}

bool NvsManager::setU16(const char* key, uint16_t value, uint16_t version) {
    return writeEntry(key, &value, sizeof(value), version);
}

bool NvsManager::setU32(const char* key, uint32_t value, uint16_t version) {
    return writeEntry(key, &value, sizeof(value), version);
}

bool NvsManager::getU8(const char* key, uint8_t& value) const {
    return readEntry(key, &value, sizeof(value), nullptr);
}

bool NvsManager::getU16(const char* key, uint16_t& value) const {
    return readEntry(key, &value, sizeof(value), nullptr);
}

bool NvsManager::getU32(const char* key, uint32_t& value) const {
    return readEntry(key, &value, sizeof(value), nullptr);
}

bool NvsManager::setString(const char* key, const char* value, uint16_t version) {
    return writeEntry(key, value, std::strlen(value) + 1, version);
}

bool NvsManager::getString(const char* key, char* buf, size_t maxLen) const {
    return readEntry(key, buf, maxLen, nullptr);
}

bool NvsManager::remove(const char* key) {
#ifdef TAKT_ESP32
    nvs_handle_t handle;
    if (nvs_open(namespace_, NVS_READWRITE, &handle) != ESP_OK) return false;
    nvs_erase_key(handle, key);
    char metaKey[40];
    snprintf(metaKey, sizeof(metaKey), "%s_meta", key);
    nvs_erase_key(handle, metaKey);
    nvs_commit(handle);
    nvs_close(handle);
    return true;
#else
    (void)key;
    return true;
#endif
}

bool NvsManager::eraseAll() {
#ifdef TAKT_ESP32
    return nvs_flash_erase() == ESP_OK;
#else
    return true;
#endif
}

bool NvsManager::backup() {
#ifdef TAKT_ESP32
    nvs_handle_t src, dst;
    if (nvs_open(namespace_, NVS_READONLY, &src) != ESP_OK) return false;
    if (nvs_open(kBackupNs, NVS_READWRITE, &dst) != ESP_OK) {
        nvs_close(src);
        return false;
    }

    nvs_iterator_t it = nullptr;
    if (nvs_entry_find("nvs", namespace_, NVS_TYPE_ANY, &it) != ESP_OK) {
        nvs_close(src);
        nvs_close(dst);
        return false;
    }

    do {
        nvs_entry_info_t info{};
        nvs_entry_info(it, &info);
        if (std::strstr(info.key, "_meta") != nullptr) continue;

        size_t len = 0;
        if (nvs_get_blob(src, info.key, nullptr, &len) != ESP_OK || len == 0) continue;

        std::vector<uint8_t> buf(len);
        if (nvs_get_blob(src, info.key, buf.data(), &len) != ESP_OK) continue;
        nvs_set_blob(dst, info.key, buf.data(), len);

        char metaKey[40];
        snprintf(metaKey, sizeof(metaKey), "%s_meta", info.key);
        NvsEntry meta{};
        size_t metaLen = sizeof(meta);
        if (nvs_get_blob(src, metaKey, &meta, &metaLen) == ESP_OK) {
            nvs_set_blob(dst, metaKey, &meta, sizeof(meta));
        }
    } while (nvs_entry_next(&it) == ESP_OK);

    nvs_set_u32(dst, kBackupMagicKey, kBackupMagic);
    nvs_commit(dst);
    nvs_close(src);
    nvs_close(dst);
    TAKT_LOGI(TAG, "Backup complete");
    return true;
#else
    return true;
#endif
}

bool NvsManager::restoreFromBackup() {
#ifdef TAKT_ESP32
    nvs_handle_t src, dst;
    uint32_t magic = 0;
    if (nvs_open(kBackupNs, NVS_READONLY, &src) != ESP_OK) return false;
    if (nvs_get_u32(src, kBackupMagicKey, &magic) != ESP_OK || magic != kBackupMagic) {
        nvs_close(src);
        return false;
    }
    if (nvs_open(namespace_, NVS_READWRITE, &dst) != ESP_OK) {
        nvs_close(src);
        return false;
    }

    nvs_iterator_t it = nullptr;
    if (nvs_entry_find("nvs", kBackupNs, NVS_TYPE_ANY, &it) != ESP_OK) {
        nvs_close(src);
        nvs_close(dst);
        return false;
    }

    do {
        nvs_entry_info_t info{};
        nvs_entry_info(it, &info);
        if (std::strcmp(info.key, kBackupMagicKey) == 0) continue;

        size_t len = 0;
        if (nvs_get_blob(src, info.key, nullptr, &len) != ESP_OK || len == 0) continue;
        std::vector<uint8_t> buf(len);
        if (nvs_get_blob(src, info.key, buf.data(), &len) != ESP_OK) continue;
        nvs_set_blob(dst, info.key, buf.data(), len);
    } while (nvs_entry_next(&it) == ESP_OK);

    nvs_commit(dst);
    nvs_close(src);
    nvs_close(dst);
    TAKT_LOGI(TAG, "Restored from backup");
    return true;
#else
    return true;
#endif
}

bool NvsManager::verifyIntegrity() const {
#ifdef TAKT_ESP32
    nvs_handle_t handle;
    if (nvs_open(namespace_, NVS_READONLY, &handle) != ESP_OK) return false;

    nvs_iterator_t it = nullptr;
    if (nvs_entry_find("nvs", namespace_, NVS_TYPE_ANY, &it) != ESP_OK) {
        nvs_close(handle);
        return true;
    }

    bool ok = true;
    do {
        nvs_entry_info_t info{};
        nvs_entry_info(it, &info);
        if (std::strstr(info.key, "_meta") != nullptr) continue;

        NvsEntry meta{};
        char metaKey[40];
        snprintf(metaKey, sizeof(metaKey), "%s_meta", info.key);
        size_t metaLen = sizeof(meta);
        if (nvs_get_blob(handle, metaKey, &meta, &metaLen) != ESP_OK) {
            ok = false;
            break;
        }

        std::vector<uint8_t> buf(meta.dataSize);
        size_t dataLen = meta.dataSize;
        if (nvs_get_blob(handle, info.key, buf.data(), &dataLen) != ESP_OK) {
            ok = false;
            break;
        }
        if (computeCrc(buf.data(), dataLen) != meta.crc32) {
            ok = false;
            break;
        }
    } while (nvs_entry_next(&it) == ESP_OK);

    nvs_close(handle);
    return ok;
#else
    return true;
#endif
}

} // namespace takt
