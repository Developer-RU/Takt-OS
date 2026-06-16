# Security Policy

## Supported versions

| Version | Supported          |
| ------- | ------------------ |
| 0.2.x   | :white_check_mark: |
| < 0.2   | :x:                |

## Reporting a vulnerability

**Please do not report security vulnerabilities through public GitHub issues.**

Instead, report them by one of the following:

1. Open a [GitHub Security Advisory](https://github.com/Developer-RU/Takt-OS/security/advisories/new) (preferred for public repos)
2. Email **p.masyukov@gmail.com** (Masyukov Pavel, project developer)

Include:

- Description of the vulnerability
- Steps to reproduce
- Affected versions and components
- Potential impact (e.g. remote code execution, credential leak)
- Suggested fix if you have one

We aim to acknowledge reports within **5 business days** and provide a timeline for a fix.

## Security considerations in TAKT OS

Current limitations documented for production deployments:

| Area | Status |
|------|--------|
| Secure Boot | Not enabled by default |
| Flash encryption | Not enabled by default |
| OTA signature verification | Partial — validate before production |
| WiFi credentials | Stored in NVS — protect physical access |
| Default MQTT broker | Demo only — configure for production |
| BLE DFU | Stub / requires NimBLE configuration |

Before deploying to critical infrastructure:

1. Enable ESP-IDF Secure Boot and flash encryption where appropriate.
2. Sign OTA images and verify in `FirmwareCache` / recovery layer.
3. Change default WiFi SSID/password and MQTT endpoints.
4. Review [docs/certification.md](docs/certification.md) test matrix.

## Attribution

If you publish research or advisories based on reports to this project, please credit TAKT OS and link to the fixed release.

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
