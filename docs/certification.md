# TAKT OS Certification Package

## Scope

TAKT OS v0.2.0 for ESP32-based industrial controllers (reference demo, IoT gateways).

## Test Matrix

| Test | Method | Pass Criteria |
|------|--------|---------------|
| Boot reliability | 1000 power cycles | 100% boot to app |
| OTA success rate | 200 OTA cycles | > 99.5% success |
| Rollback | 50 forced failures | 100% recovery |
| Takt overrun | 24h full load | 0 critical overruns |
| Heap stability | 30-day stress | No leak > 1 KB |
| NVS corruption | Injected bit flips | Auto-restore from backup |
| Watchdog | Induced hang | Recovery < 10s |

## EMC/ESD

- IEC 61000-4-2 ESD: ±8 kV contact, ±15 kV air
- IEC 61000-4-4 EFT: ±2 kV power port
- EN 55032 Class B emissions

## Documentation Set

1. Software architecture (docs/architecture.md)
2. API reference (docs/api_reference.md)
3. Risk assessment (this document)
4. Test reports (CI artifacts)
5. OTA procedure (docs/recovery.md)

## MTBF Target

> 8760 hours (1 year) continuous operation at 45°C ambient.

## Traceability

| Requirement | Implementation | Test |
|-------------|----------------|------|
| Deterministic scheduling | Scheduler | test_scheduler.cpp |
| OTA rollback | FirmwareCache | test_firmware_cache.cpp |
| Boot loop protection | Bootloader bootCount | test_bootloader.cpp |
| Data integrity | NvsManager CRC + backup | test_nvs.cpp |

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
