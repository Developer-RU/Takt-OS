# Contributing to TAKT OS

Thank you for your interest in contributing. This project is open source under the [Apache License 2.0](LICENSE).

**Maintainer:** Masyukov Pavel · [p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)

## Before you start

1. Read [docs/architecture.md](docs/architecture.md) and [docs/developer_guide.md](docs/developer_guide.md).
2. Check [open issues](https://github.com/Developer-RU/Takt-OS/issues) to avoid duplicate work.
3. For large changes, open an issue first to discuss design.

## Development setup

### ESP32 (primary target)

```bash
# ESP-IDF >= 5.0
source $IDF_PATH/export.sh   # or PlatformIO framework export
cd examples/demo_controller
idf.py set-target esp32
idf.py build flash monitor
```

### PlatformIO (VS Code / Cursor)

Open `Takt-OS.code-workspace`, install **PlatformIO IDE**, use Build / Upload / Monitor.
See [docs/platformio_vscode.md](docs/platformio_vscode.md).

### Host unit tests

```bash
cd tests
cmake -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Do not commit

- `build/`, `.pio/`, `sdkconfig`, `sdkconfig.old` — generated locally, may contain absolute paths
- Personal serial ports or IDE paths in source files — use `PORT`, `$IDF_PATH`, `${workspaceFolder}`


- **C++17**, no extensions (`-std=c++17`)
- Match existing naming: `PascalCase` classes, `camelCase` methods, `snake_case` files in some layers
- Modules implement `takt::IModule`; services and middleware follow existing patterns
- Keep changes focused; avoid unrelated refactors in the same PR
- Prefer self-explanatory code; comment only non-obvious business logic

## Project layout

| Path | Add code here when… |
|------|---------------------|
| `kernel/` | Core scheduler, events, timers, storage |
| `drivers/` | Hardware abstraction (GPIO, UART, ADC) |
| `middleware/` | Reusable protocol / connectivity modules |
| `services/` | Application-level services (OTA, telemetry) |
| `examples/` | Demo firmware only |
| `tests/` | Host unit tests |

ESP-IDF uses **folder name** as component name — `REQUIRES` in `CMakeLists.txt` must match the directory name (e.g. `kernel`, not `takt_kernel`).

## Pull request process

1. Fork the repository and create a branch from `main`.
2. Make your changes with clear commit messages (imperative mood, e.g. `Add Modbus TCP server module`).
3. Ensure builds pass:
   - `idf.py build` in `examples/demo_controller`
   - Host tests if you touched kernel logic
4. Update documentation if you change public APIs or behavior.
5. Add an entry to [CHANGELOG.md](CHANGELOG.md) under `[Unreleased]`.
6. Open a PR with:
   - Summary of what and why
   - Test plan (commands run, hardware used)
   - Link to related issue

## Licensing and attribution

By contributing, you agree that your contributions are licensed under the Apache License 2.0.

If you copy code from another project:

- Ensure the license is compatible (Apache 2.0, MIT, BSD typically are)
- Add attribution in [NOTICE](NOTICE) and file headers where appropriate
- Do not copy proprietary or GPL code without explicit approval

## Reporting bugs

Use the [bug report template](.github/ISSUE_TEMPLATE/bug_report.yml). Include board, ESP-IDF version, steps to reproduce, and serial log.

## Security

See [SECURITY.md](SECURITY.md) for responsible disclosure.

## Code of conduct

This project follows the [Contributor Covenant](CODE_OF_CONDUCT.md). Be respectful and constructive.

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
