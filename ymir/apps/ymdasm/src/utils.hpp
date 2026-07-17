#pragma once

#include <fmt/format.h>

#include <concepts>
#include <optional>
#include <string_view>

template <std::unsigned_integral T>
std::optional<T> ParseHex(std::string_view opcode) {
    static constexpr size_t kHexOpcodeLength = sizeof(T) * 2;

    if (opcode.size() > kHexOpcodeLength) {
        fmt::println("Value \"{}\" exceeds maximum length of {} hex digits", opcode, kHexOpcodeLength);
        return std::nullopt;
    }

    T value = 0;
    for (char c : opcode) {
        if (c >= '0' && c <= '9') {
            value = (value << 4ull) + c - '0';
        } else if (c >= 'A' && c <= 'F') {
            value = (value << 4ull) + c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            value = (value << 4ull) + c - 'a' + 10;
        } else {
            fmt::println("Value \"{}\" is not a valid hexadecimal number", opcode);
            return std::nullopt;
        }
    }
    return value;
}
