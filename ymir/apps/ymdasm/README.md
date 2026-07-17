# ymdasm
Ymir disassembly tool, supporting SH-2, Motorola 68000, Saturn SCU DSP and SCSP DSP ISAs.



## Usage

Type `ymdasm --help` to get help about the command.

```sh
Ymir disassembly tool
Version 0.1.0
Usage:
  ymdasm [OPTION...] <isa> {<program opcodes>|[<offset> [<length>]]}

  -h, --help              Display this help text.
  -C, --color color_mode  Color text output (stdout only): none, basic,
                          truecolor (default: none)
  -i, --input-file path   Disassemble code from the specified file. Omit to
                          disassemble command line arguments.
  -o, --origin address    Origin (base) address of the disassembled code.
                          (default: 0)
  -a, --hide-addresses    Hide addresses from disassembly listing.
  -c, --hide-opcodes      Hide opcodes from disassembly listing.

  <isa> specifies an instruction set architecture to disassemble:
    sh2, sh-2     Hitachi/Renesas SuperH-2
    m68k, m68000  Motorola 68000
    scudsp        SCU (Saturn Control Unit) DSP
    scspdsp       SCSP (Saturn Custom Sound Processor) DSP
    scspdspraw    SCSP (Saturn Custom Sound Processor) DSP (raw dissasembly)
  This argument is case-insensitive.

  When disassembling command line arguments, <program opcodes> specifies the
  hexadecimal opcodes to disassemble.

  When disassembling from a file, <offset> specifies the offset from the start
  of the file and <length> determines the number of bytes to disassemble.
  Both parameters are specified in hexadecimal.
  <length> is truncated down to the nearest multiple of the opcode size.
  If <offset> is omitted, ymdasm disassembles from the start of the file.
  If <length> is omitted, ymdasm disassembles until the end of the file.

  SuperH-2 opcodes can be prefixed with ! to force them to be decoded as delay
  slot instructions or _ to force instructions in delay slots to be decoded as
  regular instructions.
```

The instruction set architecture argument is case-insensitive.

`ymdasm` does not assume a default ISA; you must always specify one of the supported ISA arguments to disassemble code.

`<program opcodes>` is a sequence of hexadecimal opcodes. Each space-separated value is interpreted as one opcode.
Partial opcodes are left padded with zeros to full opcodes, and opcodes that exceed the opcode length of the ISA are
rejected. For example, with a 16-bit ISA, `1 22 333 4444` contains opcodes `0001`, `0022`, `0333` and `4444`, and
`55555` would cause an error.



### Hitachi/Renesas SH-2 (`sh2`)

The [Hitachi/Renesas SH-2](https://en.wikipedia.org/wiki/SuperH) is a big-endian architecture with fixed-length 16-bit
opcodes.

You can find an opcode table [here](https://shared-ptr.com/sh_insns.html).

For this ISA, you can modify opcode decoding by prefixing opcodes with one of these symbols:
- `!`: force decoding as a delay slot instruction
- `_`: force decoding as a non-delay slot instruction

For example:
```sh
ymdasm sh2 000B 0009 000B _0009 !0009 !000B
```
results in:
```
00000000  000B  rts
00000002  0009  > nop
00000004  000B  rts
00000006  0009  nop                                       ; non-delay slot override
00000008  0009  > nop                                     ; delay slot override
0000000A  000B  > (illegal)                               ; delay slot override
```

These modifiers cannot be used when disassembling files. The first instruction is assumed to not be in a delay slot.



### Motorola 68000 (`m68k`)

The [Motorola 68000 instruction set](https://en.wikipedia.org/wiki/Motorola_68000) is big-endian with variable-length
16-bit opcodes.

You can find an opcode table [here](http://goldencrystal.free.fr/M68kOpcodes-v2.3.pdf).



### SCU DSP (`scudsp`)

The Saturn SCU (System Control Unit) DSP uses 32-bit big-endian [VLIW](https://en.wikipedia.org/wiki/Very_long_instruction_word)
opcodes and is capable of executing up to 6 instructions at once. It is often used to offload 3D geometry calculations.

You can find an opcode table [here](https://github.com/srg320/Saturn_hw/blob/main/SCU/SCU%20DSP.xlsx).



### SCSP DSP (`scspdsp`, `scspdspraw`)

The SCSP (Saturn Custom Sound Processor) DSP uses 64-bit big-endian instructions specialized for audio processing.

The best source of information about this ISA is Neill Corlett's research on the Dreamcast's AICA (the successor of the
SCSP), which has been mirrored [here](https://raw.githubusercontent.com/Senryoku/dreamcast-docs/refs/heads/master/AICA/DOCS/myaica.txt).

The raw disassembly (`scspdspraw`) prints out the raw values of the instruction's parameters.
The regular disassembly (`scspdsp`) converts the raw parameters into a human-readable format.



## Examples

Disassemble SH-2 code:
```sh
ymdasm sh2 3233 8BF8 000B
```
```
00000000  3233  cmp/ge    r3, r2
00000002  8BF8  bf        #0xFFFFFFF4
00000004  000B  > rts
```

Change origin:
```sh
ymdasm sh2 -o 6000180 3233 8BF8 000B
```
```
06000180  3233  cmp/ge    r3, r2
06000182  8BF8  bf        #0x6000174
06000184  000B  > rts
```

Force delay slot for first SH-2 instruction:
```sh
ymdasm sh2 !000B 000B
```
```
00000000  000B  > (illegal)
00000002  000B  rts
```

Disassemble Motorola 68000 code:
```sh
ymdasm m68k -o 10000 807b f566 e82f
```
```
00010000  807b f566 e82f  or.w     ([#0x1e831, pc]), d0
```
