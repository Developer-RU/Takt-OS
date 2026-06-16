# Licensing guide

TAKT OS is distributed under the [Apache License 2.0](../LICENSE).

## For users (copying TAKT OS)

1. Keep `LICENSE` and `NOTICE` in your distribution.
2. Add attribution in README, docs, or product **About** screen.
3. Document modifications.
4. Respect ESP-IDF and other third-party licenses in [NOTICE](../NOTICE).

Example `README` snippet:

```markdown
## Third-party software

This firmware is based on [TAKT OS](https://github.com/Developer-RU/Takt-OS),
Copyright 2024-2026 Masyukov Pavel, Apache License 2.0.
See NOTICE for Espressif ESP-IDF and other components.
```

## For contributors

- New code you submit is Apache 2.0 (see [CONTRIBUTING.md](../CONTRIBUTING.md)).
- Do not commit proprietary or incompatible-licensed code.
- If you port code from another open-source project, verify license compatibility and add entry to `NOTICE`.

## Source file headers (recommended)

For **new** files:

```cpp
// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS
```

For **adapted** third-party files, retain original copyright and add:

```cpp
// Portions Copyright … Original Project
// Modifications Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0 (or original license if required)
```

## Firmware binaries

Distributing compiled `.bin` files still requires license compliance for **source** offers if you distribute object code under Apache 2.0 — typically satisfied by pointing to your fork's public repository.

## Trademarks

Apache 2.0 does not grant trademark rights to "TAKT OS".

## More

- [Licensing guide](licensing.md) (this document)
- [Apache 2.0 FAQ](https://www.apache.org/foundation/license-faq.html)

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
