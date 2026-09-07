#!/usr/bin/env python3
"""Generate a tiny ORIGINAL Sega Saturn homebrew ISO (Mode-1 / 2048).

Compatible with Ymir media::loader::iso (web bridge loadDisc).
Not a commercial dump. Security/area regions are original stubs.
"""
from __future__ import annotations

import argparse
import struct
from pathlib import Path

SECTOR = 2048
IP_SECTORS = 16
IP_SIZE = IP_SECTORS * SECTOR


def sh2_program() -> bytes:
    """Hand-assembled SH-2: enable VDP2 display + bright magenta BACK color."""
    ops: list[int] = []
    labels: dict[str, int] = {}
    fixups: list[tuple[int, str, str]] = []

    def emit(op: int) -> None:
        ops.append(op & 0xFFFF)

    def label(name: str) -> None:
        labels[name] = len(ops) * 2

    def mov_l_pc(rn: int, lab: str) -> None:
        fixups.append((len(ops), "mov_l", lab))
        emit(0xD000 | (rn << 8))

    def mov_w_pc(rn: int, lab: str) -> None:
        fixups.append((len(ops), "mov_w", lab))
        emit(0x9000 | (rn << 8))

    label("entry")
    mov_l_pc(0, "lit_vdp2")
    mov_w_pc(1, "lit_tvmd")
    emit(0x2011)  # mov.w r1, @r0  ; TVMD
    mov_l_pc(0, "lit_vdp2")
    emit(0x7020)  # add #0x20, r0  ; BGON
    emit(0xE100)  # mov #0, r1
    emit(0x2011)  # mov.w r1, @r0
    mov_l_pc(0, "lit_bkta")
    emit(0xE100)
    emit(0x2011)  # BKTAU=0
    emit(0x7002)  # add #2, r0
    emit(0x2011)  # BKTAL=0
    mov_l_pc(0, "lit_vram")
    mov_w_pc(1, "lit_color")
    emit(0x2011)
    label("spin")
    fixups.append((len(ops), "bra", "spin"))
    emit(0xA000)
    emit(0x0009)

    while (len(ops) * 2) % 4 != 0:
        emit(0x0009)

    def place_long(name: str, value: int) -> None:
        labels[name] = len(ops) * 2
        ops.append((value >> 16) & 0xFFFF)
        ops.append(value & 0xFFFF)

    def place_word(name: str, value: int) -> None:
        labels[name] = len(ops) * 2
        ops.append(value & 0xFFFF)

    place_long("lit_vdp2", 0x25F80000)
    place_long("lit_bkta", 0x25F800AC)
    place_long("lit_vram", 0x25E00000)
    place_word("lit_tvmd", 0x8000)
    place_word("lit_color", 0x7C1F)

    for idx, kind, lab in fixups:
        pc = idx * 2
        target = labels[lab]
        rn = (ops[idx] >> 8) & 0xF
        if kind == "mov_l":
            disp = (target - (pc + 4)) // 4
            assert 0 <= disp <= 255, (lab, disp)
            ops[idx] = 0xD000 | (rn << 8) | disp
        elif kind == "mov_w":
            disp = (target - (pc + 4)) // 2
            assert 0 <= disp <= 255, (lab, disp)
            ops[idx] = 0x9000 | (rn << 8) | disp
        elif kind == "bra":
            disp = (target - (pc + 4)) // 2
            assert -2048 <= disp <= 2047
            ops[idx] = 0xA000 | (disp & 0xFFF)

    out = bytearray()
    for op in ops:
        out.extend(struct.pack(">H", op))
    return bytes(out)


def build_system_id() -> bytes:
    buf = bytearray(b" " * 256)

    def put(off: int, s: str, width: int) -> None:
        b = s.encode("ascii")[:width]
        buf[off : off + width] = b + b" " * (width - len(b))

    put(0x00, "SEGA SEGASATURN", 16)
    put(0x10, "YmirWebHB", 16)
    put(0x20, "YMIR-HB01", 10)
    put(0x2A, "V1.000", 6)
    put(0x30, "20260907", 8)
    put(0x38, "CD-1/1", 8)
    put(0x40, "JUE", 10)
    put(0x50, "J", 16)
    put(0x60, "YMIR WEB HOMEBREW COLOR", 0x70)
    struct.pack_into(">I", buf, 0xE0, IP_SIZE)
    struct.pack_into(">I", buf, 0xE8, 0x1000)
    struct.pack_into(">I", buf, 0xEC, 0x1000)
    struct.pack_into(">I", buf, 0xF0, 0x06010000)
    struct.pack_into(">I", buf, 0xF4, 0x00001000)
    return bytes(buf)


def build_ip_bin(aip: bytes) -> bytes:
    ip = bytearray(IP_SIZE)
    ip[0:256] = build_system_id()
    # Original stubs (NOT Sega SYS_SEC / SYS_ARE* copyrighted objects).
    ip[0x100:0x100 + 0xD00] = b"\x00" * 0xD00
    rts_nop = struct.pack(">HH", 0x000B, 0x0009)
    for off in (0xE00, 0xE20, 0xE40, 0xE60):
        ip[off : off + 4] = rts_nop
    aip_off = 0x1000
    ip[aip_off : aip_off + len(aip)] = aip
    # Trampoline at 0x100 -> AIP (for soft-check paths)
    disp = (aip_off - (0x100 + 4)) // 2
    struct.pack_into(">H", ip, 0x100, 0xA000 | (disp & 0xFFF))
    struct.pack_into(">H", ip, 0x102, 0x0009)
    return bytes(ip)


def both_endian32(v: int) -> bytes:
    return struct.pack("<I", v) + struct.pack(">I", v)


def both_endian16(v: int) -> bytes:
    return struct.pack("<H", v) + struct.pack(">H", v)


def make_dir_record(extent: int, data_len: int, name: bytes, is_dir: bool) -> bytes:
    name_len = len(name)
    base_len = 33 + name_len + (0 if (name_len % 2) else 1)
    rec = bytearray(base_len)
    rec[0] = base_len
    rec[2:10] = both_endian32(extent)
    rec[10:18] = both_endian32(data_len)
    rec[25] = 0x02 if is_dir else 0x00
    rec[28:32] = both_endian16(1)
    rec[32] = name_len
    rec[33 : 33 + name_len] = name
    return bytes(rec)


def build_iso(ip: bytes, payload: bytes) -> bytes:
    assert len(ip) == IP_SIZE
    root_lba, file_lba = 18, 19
    file_size = len(payload)
    file_sectors = max((file_size + SECTOR - 1) // SECTOR, 1)
    total_sectors = file_lba + file_sectors

    dot = make_dir_record(root_lba, SECTOR, b"\x00", True)
    dotdot = make_dir_record(root_lba, SECTOR, b"\x01", True)
    file_rec = make_dir_record(file_lba, file_size, b"0.BIN;1", False)
    root_data = bytearray(SECTOR)
    off = 0
    for rec in (dot, dotdot, file_rec):
        root_data[off : off + len(rec)] = rec
        off += len(rec)

    pvd = bytearray(SECTOR)
    pvd[0] = 1
    pvd[1:6] = b"CD001"
    pvd[6] = 1
    pvd[8:40] = b"SEGA SATURN".ljust(32)
    pvd[40:72] = b"YMIRWEBHB".ljust(32)
    pvd[80:88] = both_endian32(total_sectors)
    pvd[120:124] = both_endian16(1)
    pvd[124:128] = both_endian16(1)
    pvd[128:132] = both_endian16(SECTOR)
    pvd[156 : 156 + len(dot)] = dot
    pvd[190:574] = b" " * (574 - 190)
    pvd[881] = 1

    term = bytearray(SECTOR)
    term[0] = 255
    term[1:6] = b"CD001"
    term[6] = 1

    file_data = payload + b"\x00" * (file_sectors * SECTOR - file_size)
    img = b"".join([ip, bytes(pvd), bytes(term), bytes(root_data), file_data])
    assert len(img) % SECTOR == 0
    return img


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("-o", "--out-dir", type=Path, default=Path(__file__).resolve().parent)
    args = ap.parse_args()
    out: Path = args.out_dir
    out.mkdir(parents=True, exist_ok=True)
    prog = sh2_program()
    ip = build_ip_bin(prog)
    iso = build_iso(ip, prog)
    (out / "0.bin").write_bytes(prog)
    (out / "ip.bin").write_bytes(ip)
    iso_path = out / "ymir_homebrew_color.iso"
    iso_path.write_bytes(iso)
    bin_path = out / "ymir_homebrew_color.bin"
    bin_path.write_bytes(iso)
    (out / "ymir_homebrew_color.cue").write_text(
        f'FILE "{bin_path.name}" BINARY\n  TRACK 01 MODE1/2048\n    INDEX 01 00:00:00\n'
    )
    print(f"wrote {iso_path} ({len(iso)} bytes, {len(iso)//SECTOR} sectors)")
    print(f"SH-2 program {len(prog)} bytes; IP.BIN {len(ip)} bytes")


if __name__ == "__main__":
    main()
