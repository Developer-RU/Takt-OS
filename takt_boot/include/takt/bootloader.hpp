#pragma once

#include <cstddef>
#include <cstdint>

namespace takt::boot {

/// Boot mode selected by GPIO, RTC memory, or watchdog trigger.
enum class BootMode : uint8_t {
    Normal       = 0, ///< Boot main firmware
    Recovery     = 1, ///< Boot recovery partition
    OtaPending   = 2, ///< Verify and activate OTA image
    FactoryReset = 3, ///< Erase NVS, boot recovery
    Emergency    = 4, ///< Minimal UART console, no app
};

/// Boot decision stored in RTC_NOINIT_ATTR memory — survives soft reset.
struct BootConfig {
    uint32_t magic;       ///< 0xB007C0DE
    BootMode mode;
    uint8_t  bootCount;   ///< Incremented each boot, reset on successful app start
    uint8_t  otaSlot;     ///< Target OTA slot
    uint32_t crc32;
};

static constexpr uint32_t kBootConfigMagic = 0xB007C0DE;

/// Minimal bootloader — runs before main firmware.
///
/// Responsibilities:
///   - Validate firmware image (magic, CRC, size)
///   - Select boot mode (normal / recovery / OTA / emergency)
///   - Jump to appropriate partition
///   - Independent of main firmware (own partition)
class Bootloader {
public:
    /// Entry point called from ESP-IDF second-stage bootloader hook
    /// or as the first code in the bootloader partition.
    [[noreturn]] static void entry();

    /// Determine boot mode from GPIO pins, RTC config, and image validity.
    static BootMode determineBootMode();

    /// Validate firmware in the given slot.
    static bool validateFirmware(uint8_t slot);

    /// Jump to firmware at the given flash offset. Does not return.
    [[noreturn]] static void jumpToFirmware(uint32_t flashOffset);

    /// Enter recovery mode.
    [[noreturn]] static void enterRecovery();

    /// Enter emergency UART console.
    [[noreturn]] static void enterEmergency();

    /// Read/write boot config in RTC memory.
    static bool loadBootConfig(BootConfig& config);
    static void saveBootConfig(const BootConfig& config);

    /// Mark successful boot (resets bootCount).
    static void markBootSuccessful();

    /// Schedule OTA boot on next reset.
    static void requestOtaBoot(uint8_t slot);

private:
    static uint32_t crc32Config(const void* data, size_t len);
};

} // namespace takt::boot
