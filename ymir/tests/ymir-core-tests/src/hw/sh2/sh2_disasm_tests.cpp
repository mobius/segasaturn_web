#include <catch2/catch_test_macros.hpp>

#include <ymir/hw/sh2/sh2_disasm.hpp>

using namespace ymir;

TEST_CASE("SH2 disassembler smoke tests", "[sh2][disasm][smoke]") {
    SECTION("mov.l @(0x0600035C), r6") {
        const uint32 pc = 0x0600035A;
        const auto &disasm = sh2::Disassemble(0xD600);

        REQUIRE(disasm.mnemonic == sh2::Mnemonic::MOV);
        CHECK(disasm.validInDelaySlot == true);
        CHECK(disasm.opSize == sh2::OperandSize::Long);
        CHECK(disasm.op1.type == sh2::Operand::Type::AtDispPCWordAlign);
        CHECK(disasm.op1.immDisp == 4);
        CHECK(disasm.op2.type == sh2::Operand::Type::Rn);
        CHECK(disasm.op2.reg == 6);
        CHECK((pc & ~3) + disasm.op1.immDisp == 0x0600035C);
    }
}
