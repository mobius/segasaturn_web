#pragma once

#include "ansi.hpp"

struct Colors {
    const char *address;
    const char *bytes;

    const char *delaySlot;       // SH2 delay slot prefix
    const char *mnemonic;        // All except NOP
    const char *nopMnemonic;     // NOP
    const char *illegalMnemonic; // (illegal)
    const char *sizeSuffix;      // SH2/M68K size suffixes: b w l
    const char *cond;            // Conditions: eq, ne, z, nz, T0, ...

    const char *immediate;   // #0x1234
    const char *opUse;       // Used operands
    const char *opRead;      // Read operands
    const char *opWrite;     // Written operands
    const char *opReadWrite; // Read and written operands

    const char *oper; // Operand (,); size suffix (.); operators (+, -, =, etc.)

    const char *addrInc; // SH2 and M68K address increment (@Rn+, (An)+)
    const char *addrDec; // SH2 and M68K address increment (@-Rn, -(An))

    const char *comment; // Comments

    const char *reset; // Color reset sequence
};

inline Colors kNoColors = {
    .address = "",
    .bytes = "",

    .delaySlot = "",
    .mnemonic = "",
    .nopMnemonic = "",
    .illegalMnemonic = "",
    .sizeSuffix = "",
    .cond = "",

    .immediate = "",
    .opUse = "",
    .opRead = "",
    .opWrite = "",
    .opReadWrite = "",

    .oper = "",

    .addrInc = "",
    .addrDec = "",

    .comment = "",

    .reset = "",
};

inline Colors kBasicColors = {
    .address = ANSI_FGCOLOR_WHITE,
    .bytes = ANSI_FGCOLOR_WHITE,

    .delaySlot = ANSI_FGCOLOR_BLUE,
    .mnemonic = ANSI_FGCOLOR_BRIGHT_CYAN,
    .nopMnemonic = ANSI_FGCOLOR_CYAN,
    .illegalMnemonic = ANSI_FGCOLOR_BRIGHT_RED,
    .sizeSuffix = ANSI_FGCOLOR_BLUE,
    .cond = ANSI_FGCOLOR_MAGENTA,

    .immediate = ANSI_FGCOLOR_BRIGHT_YELLOW,
    .opUse = ANSI_FGCOLOR_BRIGHT_CYAN,
    .opRead = ANSI_FGCOLOR_BRIGHT_GREEN,
    .opWrite = ANSI_FGCOLOR_BRIGHT_MAGENTA,
    .opReadWrite = ANSI_FGCOLOR_BRIGHT_YELLOW,

    .oper = ANSI_FGCOLOR_BRIGHT_BLACK,

    .addrInc = ANSI_FGCOLOR_GREEN,
    .addrDec = ANSI_FGCOLOR_RED,

    .comment = ANSI_FGCOLOR_BRIGHT_BLACK,

    .reset = ANSI_RESET,
};

inline Colors kTrueColors = {
    .address = ANSI_FGCOLOR_24B(217, 216, 237),
    .bytes = ANSI_FGCOLOR_24B(237, 236, 216),

    .delaySlot = ANSI_FGCOLOR_24B(96, 112, 156),
    .mnemonic = ANSI_FGCOLOR_24B(173, 216, 247),
    .nopMnemonic = ANSI_FGCOLOR_24B(66, 81, 92),
    .illegalMnemonic = ANSI_FGCOLOR_24B(247, 191, 173),
    .sizeSuffix = ANSI_FGCOLOR_24B(143, 159, 207),
    .cond = ANSI_FGCOLOR_24B(155, 146, 212),

    .immediate = ANSI_FGCOLOR_24B(221, 247, 173),
    .opUse = ANSI_FGCOLOR_24B(247, 240, 173),
    .opRead = ANSI_FGCOLOR_24B(173, 247, 206),
    .opWrite = ANSI_FGCOLOR_24B(215, 173, 247),
    .opReadWrite = ANSI_FGCOLOR_24B(247, 206, 173),

    .oper = ANSI_FGCOLOR_24B(186, 191, 194),

    .addrInc = ANSI_FGCOLOR_24B(147, 194, 155),
    .addrDec = ANSI_FGCOLOR_24B(194, 159, 147),

    .comment = ANSI_FGCOLOR_24B(151, 154, 156),

    .reset = ANSI_RESET,
};