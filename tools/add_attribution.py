#!/usr/bin/env python3
"""Add project attribution footer to markdown and headers to source files."""

from __future__ import annotations

import os
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

FOOTER = (
    "\n\n---\n\n"
    "**TAKT OS** — Developer: **Masyukov Pavel** "
    "([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · "
    "License: [Apache License 2.0]"
    "(https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · "
    "[Source](https://github.com/Developer-RU/Takt-OS)\n"
)

SOURCE_HEADER = (
    "// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>\n"
    "// SPDX-License-Identifier: Apache-2.0\n"
    "//\n"
    "// TAKT OS — https://github.com/Developer-RU/Takt-OS\n"
    "\n"
)

SKIP_MD = {
    ROOT / "LICENSE",
}

SKIP_DIRS = {".git", "build", ".pio", "managed_components", "node_modules"}


def should_skip_dir(name: str) -> bool:
    return name in SKIP_DIRS


def add_md_footer(path: Path) -> bool:
    if path in SKIP_MD:
        return False
    text = path.read_text(encoding="utf-8")
    if "Masyukov Pavel" in text:
        return False
    path.write_text(text.rstrip() + FOOTER, encoding="utf-8")
    return True


def add_source_header(path: Path) -> bool:
    text = path.read_text(encoding="utf-8")
    if "Masyukov Pavel" in text[:400]:
        return False
    path.write_text(SOURCE_HEADER + text, encoding="utf-8")
    return True


def add_platformio_header(path: Path) -> bool:
    text = path.read_text(encoding="utf-8")
    if "Masyukov Pavel" in text:
        return False
    block = (
        "; Developer: Masyukov Pavel <p.masyukov@gmail.com>\n"
        "; License: Apache License 2.0 — https://github.com/Developer-RU/Takt-OS\n"
        "\n"
    )
    path.write_text(block + text, encoding="utf-8")
    return True


def main() -> None:
    md_updated = []
    src_updated = []
    pio_updated = []

    for dirpath, dirnames, filenames in os.walk(ROOT):
        dirnames[:] = [d for d in dirnames if not should_skip_dir(d)]
        for name in filenames:
            path = Path(dirpath) / name
            rel = path.relative_to(ROOT)
            if name.endswith(".md"):
                if add_md_footer(path):
                    md_updated.append(str(rel))
            elif name.endswith((".hpp", ".cpp", ".h", ".c")):
                if add_source_header(path):
                    src_updated.append(str(rel))
            elif name == "platformio.ini":
                if add_platformio_header(path):
                    pio_updated.append(str(rel))

    print(f"Markdown: {len(md_updated)}")
    for p in md_updated:
        print(f"  {p}")
    print(f"Source: {len(src_updated)}")
    print(f"PlatformIO: {len(pio_updated)}")
    for p in pio_updated:
        print(f"  {p}")


if __name__ == "__main__":
    main()
