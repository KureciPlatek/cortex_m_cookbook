#!/usr/bin/env python3
"""
Sweep RP2350 SRAM (0x20000000 .. 0x2007FFFF) in batches of 64.

Output (one line per address):   0xADDRESS;0xVALUE   (or 0xADDRESS;NULL)

Latency model (measured): the reply is ONE FULL BATCH behind. The values
read while requesting batch k actually belong to batch k-1's addresses
(the RP2350 buffers 64 and flushes them when the next batch arrives). Plus
a byte-level offset in the stream.

Handling:
  - byte offset  -> parse by locking on frame markers [0x01 ... 0x0A],
  - one-batch lag -> pair THIS read's values with the PREVIOUS batch's
    addresses (one-batch pipeline),
  - flush         -> after the last real batch, send one dummy batch so the
    last real batch's values come out,
  - missing value -> written as NULL.

Reply frame: [0x01][0x00][0x00][0x00][value][0x0A]   value = index 4
"""

import sys
import serial            # pip install pyserial

PORT        = "/dev/ttyUSB0"
BAUD        = 921600
CMD         = 0x01
TERM        = 0x0A
SRAM_LO     = 0x20000000
SRAM_HI     = 0x20080000          # exclusive; last valid = 0x2007FFFF
BATCH       = 64
OUTPUT_FILE = "sram_dump.txt"
DUMMY_ADDR  = SRAM_LO

REPLY_FRAME  = 6
VALUE_INDEX  = 4
START_BYTE   = 0x01
END_BYTE     = 0x0A
QUIET_GAP    = 0.02


def frame(addr: int) -> bytes:
    return bytes([CMD]) + addr.to_bytes(4, "little") + bytes([TERM])


def read_until_quiet(ser) -> bytes:
    ser.timeout = QUIET_GAP
    out = bytearray()
    while True:
        b = ser.read(512)
        if not b:
            break
        out += b
    return bytes(out)


def extract_values(buf: bytes):
    """Marker-locked parse -> list of value bytes (absorbs byte offset)."""
    vals = []
    i, n = 0, len(buf)
    while i + REPLY_FRAME <= n:
        if buf[i] == START_BYTE and buf[i + REPLY_FRAME - 1] == END_BYTE:
            vals.append(buf[i + VALUE_INDEX])
            i += REPLY_FRAME
        else:
            i += 1
    return vals


def write_pairs(f, addrs, values):
    for i, a in enumerate(addrs):
        if i < len(values):
            f.write(f"0x{a:08X};0x{values[i]:02X}\n")
        else:
            f.write(f"0x{a:08X};NULL\n")


def main() -> int:
    try:
        ser = serial.Serial(PORT, BAUD, timeout=QUIET_GAP)
    except serial.SerialException as e:
        print(f"cannot open {PORT}: {e}", file=sys.stderr)
        return 1

    ser.reset_input_buffer()
    ser.reset_output_buffer()

    with open(OUTPUT_FILE, "w") as f:
        prev_addrs = None                      # one-batch pipeline
        addr = SRAM_LO
        while addr < SRAM_HI:
            end   = min(addr + BATCH, SRAM_HI) - 1
            addrs = list(range(addr, end + 1))

            print(f"[sweep] request 0x{addr:08X} - 0x{end:08X}")
            sys.stdout.flush()

            ser.write(b"".join(frame(a) for a in addrs))
            ser.flush()

            values = extract_values(read_until_quiet(ser))

            # values just read belong to the PREVIOUS batch's addresses
            if prev_addrs is not None:
                write_pairs(f, prev_addrs, values)
                f.flush()

            prev_addrs = addrs
            addr += BATCH

        # flush: one dummy batch to drain the last real batch's values
        if prev_addrs is not None:
            print(f"[sweep] flush last batch")
            ser.write(b"".join(frame(DUMMY_ADDR) for _ in prev_addrs))
            ser.flush()
            values = extract_values(read_until_quiet(ser))
            write_pairs(f, prev_addrs, values)
            f.flush()

    ser.close()
    print(f"[sweep] done -> {OUTPUT_FILE}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
