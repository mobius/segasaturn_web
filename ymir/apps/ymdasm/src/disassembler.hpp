#pragma once

#include "colors.hpp"

#include <ymir/core/types.hpp>

#include <fmt/format.h>

#include <concepts>
#include <cstdlib>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>

struct Disassembler {
    int x = 0;
    Colors colors = kNoColors;
    bool hideAddresses = false;
    bool hideOpcodes = false;

    Disassembler &PrintRaw(std::string_view text, bool incPos = true) {
        fmt::print("{}", text);
        if (incPos) {
            x += text.size();
        }
        return *this;
    }

    Disassembler &Print(const char *color, std::string_view text, bool incPos = true) {
        fmt::print("{}{}", color, text);
        if (incPos) {
            x += text.size();
        }
        return *this;
    }

    Disassembler &Align(int pos) {
        if (pos > x) {
            std::string pad(pos - x, ' ');
            PrintRaw(pad);
        }
        return *this;
    }

    template <std::unsigned_integral T>
    Disassembler &Address(T address) {
        if (!hideAddresses) {
            Print(colors.address, fmt::format("{:0{}X}  ", address, sizeof(T) * 2), false);
        }
        return *this;
    }

    template <std::unsigned_integral T>
    Disassembler &Opcode(T opcode) {
        if (!hideOpcodes) {
            Print(colors.bytes, fmt::format("{:0{}X}  ", opcode, sizeof(T) * 2), false);
        }
        return *this;
    }

    template <size_t maxOpcodes, std::unsigned_integral T>
    Disassembler &Opcodes(std::span<T> opcodes) {
        if (!hideOpcodes) {
            for (auto opcode : opcodes) {
                Print(colors.bytes, fmt::format("{:0{}X} ", opcode, sizeof(T) * 2), false);
            }
            for (int i = opcodes.size(); i < 5; i++) {
                PrintRaw(fmt::format("{:{}} ", "", sizeof(T) * 2), false);
            }
            PrintRaw(" ", false);
        }
        return *this;
    }

    Disassembler &Mnemonic(std::string_view mnemonic) {
        Print(colors.mnemonic, mnemonic);
        return *this;
    }

    Disassembler &NOP(const char *mnemonic) {
        Print(colors.nopMnemonic, mnemonic);
        return *this;
    }

    Disassembler &IllegalMnemonic(const char *mnemonic = "(illegal)") {
        Print(colors.illegalMnemonic, mnemonic);
        return *this;
    }

    Disassembler &UnknownMnemonic() {
        IllegalMnemonic("(?)");
        return *this;
    }

    Disassembler &Cond(std::string_view cond) {
        Print(colors.cond, cond);
        return *this;
    }

    Disassembler &Operator(const char *oper) {
        Print(colors.oper, oper);
        return *this;
    }

    Disassembler &Comma() {
        Operator(", ");
        return *this;
    }

    Disassembler &SizeSuffix(const char *size) {
        Operator(".");
        Print(colors.sizeSuffix, size);
        return *this;
    }

    Disassembler &OperandUse(std::string_view op) {
        Print(colors.opUse, op);
        return *this;
    }

    Disassembler &OperandRead(std::string_view op) {
        Print(colors.opRead, op);
        return *this;
    }

    Disassembler &OperandWrite(std::string_view op) {
        Print(colors.opWrite, op);
        return *this;
    }

    Disassembler &OperandReadWrite(std::string_view op) {
        Print(colors.opReadWrite, op);
        return *this;
    }

    Disassembler &Operand(std::string_view op, bool read, bool write) {
        if (read && write) {
            OperandReadWrite(op);
        } else if (write) {
            OperandWrite(op);
        } else if (read) {
            OperandRead(op);
        } else {
            OperandUse(op);
        }
        return *this;
    }

    template <std::integral T>
    Disassembler &ImmDec(T imm) {
        Print(colors.immediate, fmt::format("{:d}", imm));
        return *this;
    }

    template <std::unsigned_integral T, size_t width = 1>
    Disassembler &ImmHex(T imm, const char *prefix = "", const char *hexPrefix = "0x") {
        Print(colors.immediate, fmt::format("{}{}{:0{}X}", prefix, hexPrefix, imm, width));
        return *this;
    }

    template <std::signed_integral T, size_t width = 1>
    Disassembler &ImmHex(T imm, const char *prefix = "", const char *hexPrefix = "0x") {
        using U = std::make_unsigned_t<T>;
        const char *sign = imm < 0 ? "-" : "";
        const U uimm = static_cast<U>(std::abs(imm));
        Print(colors.immediate, fmt::format("{}{}{}{:0{}X}", sign, prefix, hexPrefix, uimm, width));
        return *this;
    }

    template <std::signed_integral T, size_t width = 1>
    Disassembler &ImmHexSignAfterPrefix(T imm, const char *prefix = "", const char *hexPrefix = "0x") {
        using U = std::make_unsigned_t<T>;
        const char *sign = imm < 0 ? "-" : "";
        const U uimm = static_cast<U>(std::abs(imm));
        Print(colors.immediate, fmt::format("{}{}{}{:0{}X}", prefix, sign, hexPrefix, uimm, width));
        return *this;
    }

    Disassembler &BitRange(uint32 high, uint32 low) {
        Operator("[").ImmDec(high).Operator(":").ImmDec(low).Operator("]");
        return *this;
    }

    Disassembler &ReadWriteSymbol(const char *symbol, bool read, bool write) {
        const auto color = read && write ? colors.opReadWrite
                           : write       ? colors.opWrite
                           : read        ? colors.opRead
                                         : colors.opUse;
        Print(color, symbol);
        return *this;
    }

    Disassembler &AddrInc() {
        Print(colors.addrInc, "+");
        return *this;
    }

    Disassembler &AddrDec() {
        Print(colors.addrDec, "-");
        return *this;
    }

    Disassembler &Comment(std::string_view comment) {
        Print(colors.comment, comment);
        return *this;
    }

    Disassembler &NewLine() {
        fmt::println("{}", colors.reset);
        x = 0;
        return *this;
    }
};
