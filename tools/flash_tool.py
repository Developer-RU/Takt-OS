#!/usr/bin/env python3
"""TAKT OS flash tool — partition and firmware management."""

import argparse
import sys

def main():
    parser = argparse.ArgumentParser(description="TAKT OS Flash Tool")
    parser.add_argument("command", choices=["info", "flash", "erase", "verify"])
    parser.add_argument("--port", default="/dev/ttyUSB0")
    parser.add_argument("--firmware", help="Firmware binary path")
    parser.add_argument("--slot", choices=["A", "B"], default="A")
    args = parser.parse_args()

    if args.command == "info":
        print("TAKT OS Flash Layout:")
        print("  Bootloader:  0x000000 - 0x00FFFF (64 KB)")
        print("  Recovery:    0x010000 - 0x04FFFF (256 KB)")
        print("  App Slot A:  0x050000 - 0x1AFFFF (1.5 MB)")
        print("  App Slot B:  0x1B0000 - 0x30FFFF (1.5 MB)")
        print("  NVS:         0x310000 - 0x31FFFF (64 KB)")
        print("  Storage:     0x320000 - 0x3FFFFF (896 KB)")
    elif args.command == "flash":
        if not args.firmware:
            print("Error: --firmware required", file=sys.stderr)
            sys.exit(1)
        print(f"Flashing {args.firmware} to slot {args.slot} on {args.port}")
    elif args.command == "erase":
        print(f"Erasing slot {args.slot}")
    elif args.command == "verify":
        print(f"Verifying slot {args.slot}")

if __name__ == "__main__":
    main()
