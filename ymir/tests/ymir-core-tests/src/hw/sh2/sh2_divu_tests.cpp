#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <ymir/hw/sh2/sh2_divu.hpp>

#include <fmt/format.h>

namespace sh2_divu {

struct TestData {
    struct Regs {
        uint32 DVSR;
        uint32 DVDNT;
        uint32 DVDNTL;
        uint32 DVDNTH;
        uint32 DVDNTUL;
        uint32 DVDNTUH;
        uint32 DVCR;

        constexpr auto operator<=>(const Regs &) const = default;
    };

    Regs input;
    Regs output32;
    Regs output64;
};

std::ostream &operator<<(std::ostream &os, TestData::Regs const &value) {
    os << fmt::format(
        "Regs{{DVSR={:08X} DVDNT={:08X} DVDNTL={:08X} DVDNTH={:08X} DVDNTUL={:08X} DVDNTUH={:08X} DVCR={:X}}}",
        value.DVSR, value.DVDNT, value.DVDNTL, value.DVDNTH, value.DVDNTUL, value.DVDNTUH, value.DVCR);
    return os;
}

struct TestSubject {
    mutable ymir::sh2::DivisionUnit divu{};

    TestData::Regs Compute32(const TestData::Regs &input) const {
        divu.DVSR = input.DVSR;
        divu.DVDNT = input.DVDNT;
        divu.DVDNTL = input.DVDNTL;
        divu.DVDNTH = input.DVDNTH;
        divu.DVDNTUL = input.DVDNTUL;
        divu.DVDNTUH = input.DVDNTUH;
        divu.DVCR.Write(input.DVCR);

        divu.Calc32();
        return {divu.DVSR, divu.DVDNT, divu.DVDNTL, divu.DVDNTH, divu.DVDNTUL, divu.DVDNTUH, divu.DVCR.Read()};
    }

    TestData::Regs Compute64(const TestData::Regs &input) const {
        divu.DVSR = input.DVSR;
        divu.DVDNT = input.DVDNT;
        divu.DVDNTL = input.DVDNTL;
        divu.DVDNTH = input.DVDNTH;
        divu.DVDNTUL = input.DVDNTUL;
        divu.DVDNTUH = input.DVDNTUH;
        divu.DVCR.Write(input.DVCR);

        divu.Calc64();
        return {divu.DVSR, divu.DVDNT, divu.DVDNTL, divu.DVDNTH, divu.DVDNTUL, divu.DVDNTUH, divu.DVCR.Read()};
    }
};

constexpr auto testdata = {
    TestData{.input = {0x00000002, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00},
             .output32 = {0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00},
             .output64 = {0x00000002, 0x80000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x00}},
    TestData{.input = {0xFFFFFFFE, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00},
             .output32 = {0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00},
             .output64 = {0xFFFFFFFE, 0x80000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x00}},
    TestData{.input = {0xFFFFFFFF, 0x80000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x00},
             .output32 = {0xFFFFFFFF, 0x80000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x00},
             .output64 = {0xFFFFFFFF, 0x80000000, 0x80000000, 0xFFFFFFFE, 0x80000000, 0xFFFFFFFE, 0x01}},
    TestData{.input = {0xFFFFFFFF, 0x80000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x02},
             .output32 = {0xFFFFFFFF, 0x80000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x02},
             .output64 = {0xFFFFFFFF, 0x00000005, 0x00000005, 0xFFFFFFFE, 0x00000005, 0xFFFFFFFE, 0x03}},
    TestData{.input = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00},
             .output32 = {0x00000000, 0x7FFFFFFF, 0x7FFFFFFF, 0x00000000, 0x7FFFFFFF, 0x00000000, 0x01},
             .output64 = {0x00000000, 0x7FFFFFFF, 0x7FFFFFFF, 0x00000000, 0x7FFFFFFF, 0x00000000, 0x01}},
    TestData{.input = {0xFFFFFFFF, 0x80000000, 0x80000000, 0xFFFFFFFF, 0x80000000, 0xFFFFFFFF, 0x00},
             .output32 = {0xFFFFFFFF, 0x80000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x00},
             .output64 = {0xFFFFFFFF, 0x80000000, 0x80000000, 0x00000000, 0x80000000, 0x00000000, 0x00}},
#include "sh2_divu_testdata.inc"
};

TEST_CASE_PERSISTENT_FIXTURE(TestSubject, "SH2 DIVU divisions are computed correctly", "[sh2][divu]") {
    const auto &testData = GENERATE(values<TestData>(testdata));

    CHECK(Compute32(testData.input) == testData.output32);
    CHECK(Compute64(testData.input) == testData.output64);
}

} // namespace sh2_divu
