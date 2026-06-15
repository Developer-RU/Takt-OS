#!/usr/bin/env python3
"""30-day stress test harness (runs accelerated cycles on host)."""

import time
import subprocess
import sys

def main():
    cycles = int(sys.argv[1]) if len(sys.argv) > 1 else 1000
    print(f"TAKT OS stress test: {cycles} cycles")
    failures = 0
    for i in range(cycles):
        result = subprocess.run(["./build/tests/takt_tests"], capture_output=True)
        if result.returncode != 0:
            failures += 1
            print(f"  FAIL cycle {i}")
        if (i + 1) % 100 == 0:
            print(f"  Progress: {i + 1}/{cycles}, failures={failures}")
    print(f"Done: {cycles} cycles, {failures} failures")
    return 1 if failures else 0

if __name__ == "__main__":
    sys.exit(main())
