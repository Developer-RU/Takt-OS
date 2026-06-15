#!/usr/bin/env python3
"""TAKT OS manufacturing UART test suite."""

import argparse
import serial
import sys
import time

COMMANDS = {
    "ping": b"PING\n",
    "stats": b"STATS\n",
    "reset": b"RESET\n",
}

def run_test(port: str, baud: int = 115200) -> int:
    print(f"Connecting to {port} @ {baud}")
    with serial.Serial(port, baud, timeout=2) as ser:
        for name, cmd in COMMANDS.items():
            ser.write(cmd)
            time.sleep(0.5)
            resp = ser.read(256)
            print(f"  [{name}] {resp.decode('utf-8', errors='replace').strip() or '(no response)'}")
    print("Manufacturing test complete")
    return 0

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="/dev/ttyUSB0")
    parser.add_argument("--baud", type=int, default=115200)
    args = parser.parse_args()
    try:
        sys.exit(run_test(args.port, args.baud))
    except serial.SerialException as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
