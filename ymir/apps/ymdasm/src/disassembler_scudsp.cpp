#include "disassembler_scudsp.hpp"

#include "fetcher.hpp"

#include <ymir/hw/scu/scu_dsp_disasm.hpp>

struct SCUDSPDisassembler {
    SCUDSPDisassembler(Disassembler &disasm)
        : disasm(disasm) {}

    SCUDSPDisassembler &Address() {
        disasm.Address(address);
        return *this;
    }

    SCUDSPDisassembler &Opcode(uint32 opcode) {
        disasm.Opcode(opcode);
        return *this;
    }

    SCUDSPDisassembler &U8(uint8 imm) {
        disasm.ImmHex(imm, "#");
        return *this;
    }

    SCUDSPDisassembler &S8(sint8 imm) {
        disasm.ImmHexSignAfterPrefix(imm, "#");
        return *this;
    }

    SCUDSPDisassembler &S32(sint32 imm) {
        disasm.ImmHexSignAfterPrefix(imm, "#");
        return *this;
    }

    SCUDSPDisassembler &U32(uint32 imm) {
        disasm.ImmHex(imm, "#");
        return *this;
    }

    SCUDSPDisassembler &Mnemonic(std::string_view mnemonic) {
        disasm.Mnemonic(mnemonic);
        return *this;
    }

    SCUDSPDisassembler &Cond(ymir::scu::SCUDSPInstruction::Cond cond) {
        disasm.Cond(ymir::scu::ToString(cond));
        return *this;
    }

    SCUDSPDisassembler &Comma() {
        disasm.Comma();
        return *this;
    }

    SCUDSPDisassembler &OperandRead(std::string_view op) {
        disasm.OperandRead(op);
        return *this;
    }

    SCUDSPDisassembler &OperandWrite(std::string_view op) {
        disasm.OperandWrite(op);
        return *this;
    }

    SCUDSPDisassembler &Instruction(const ymir::scu::SCUDSPInstruction &instr) {
        using InstrType = ymir::scu::SCUDSPInstruction::Type;

        switch (instr.type) {
        case InstrType::Operation: //
        {
            if (instr.operation.aluOp == ymir::scu::SCUDSPInstruction::ALUOp::NOP) {
                disasm.NOP("NOP");
            } else {
                Mnemonic(ymir::scu::ToString(instr.operation.aluOp));
            }
            disasm.Align(5);

            using XBusPOp = ymir::scu::SCUDSPInstruction::XBusPOp;

            switch (instr.operation.xbusPOp) {
            case XBusPOp::NOP: disasm.NOP("NOP"); break;
            case XBusPOp::MOV_MUL_P: disasm.Mnemonic("MOV ").OperandRead("MUL").Comma().OperandWrite("P"); break;
            case XBusPOp::MOV_S_P:
                Mnemonic("MOV ").OperandRead(ymir::scu::ToString(instr.operation.xbusSrc)).Comma().OperandWrite("P");
                break;
            }
            disasm.Align(17);

            if (instr.operation.xbusXOp) {
                Mnemonic("MOV ").OperandRead(ymir::scu::ToString(instr.operation.xbusSrc)).Comma().OperandWrite("X");
            } else {
                disasm.NOP("NOP");
            }
            disasm.Align(29);

            using YBusAOp = ymir::scu::SCUDSPInstruction::YBusAOp;

            switch (instr.operation.ybusAOp) {
            case YBusAOp::NOP: disasm.NOP("NOP"); break;
            case YBusAOp::CLR_A: disasm.Mnemonic("CLR ").OperandWrite("A"); break;
            case YBusAOp::MOV_ALU_A: disasm.Mnemonic("MOV ").OperandRead("ALU").Comma().OperandWrite("A"); break;
            case YBusAOp::MOV_S_A:
                Mnemonic("MOV ").OperandRead(ymir::scu::ToString(instr.operation.xbusSrc)).Comma().OperandWrite("A");
                break;
            }
            disasm.Align(41);

            if (instr.operation.ybusYOp) {
                Mnemonic("MOV ").OperandRead(ymir::scu::ToString(instr.operation.ybusSrc)).Comma().OperandWrite("Y");
            } else {
                disasm.NOP("NOP");
            }
            disasm.Align(53);

            using D1BusOp = ymir::scu::SCUDSPInstruction::D1BusOp;

            switch (instr.operation.d1BusOp) {
            case D1BusOp::NOP: disasm.NOP("NOP"); break;
            case D1BusOp::MOV_SIMM_D:
                Mnemonic("MOV ");
                switch (instr.operation.d1BusDst) {
                case ymir::scu::SCUDSPInstruction::OpDst::MC0: [[fallthrough]];
                case ymir::scu::SCUDSPInstruction::OpDst::MC1: [[fallthrough]];
                case ymir::scu::SCUDSPInstruction::OpDst::MC2: [[fallthrough]];
                case ymir::scu::SCUDSPInstruction::OpDst::MC3: S8(instr.operation.d1BusSrc.imm); break;
                case ymir::scu::SCUDSPInstruction::OpDst::RX: S8(instr.operation.d1BusSrc.imm); break;
                case ymir::scu::SCUDSPInstruction::OpDst::P: S8(instr.operation.d1BusSrc.imm); break;
                case ymir::scu::SCUDSPInstruction::OpDst::RA0: [[fallthrough]];
                case ymir::scu::SCUDSPInstruction::OpDst::WA0:
                    U32((instr.operation.d1BusSrc.imm << 2) & 0x7FF'FFFC);
                    break;
                case ymir::scu::SCUDSPInstruction::OpDst::LOP: U32(instr.operation.d1BusSrc.imm & 0xFFF); break;
                case ymir::scu::SCUDSPInstruction::OpDst::TOP: U8(instr.operation.d1BusSrc.imm); break;
                case ymir::scu::SCUDSPInstruction::OpDst::CT0: [[fallthrough]];
                case ymir::scu::SCUDSPInstruction::OpDst::CT1: [[fallthrough]];
                case ymir::scu::SCUDSPInstruction::OpDst::CT2: [[fallthrough]];
                case ymir::scu::SCUDSPInstruction::OpDst::CT3: U8(instr.operation.d1BusSrc.imm & 0x3F); break;
                default: S8(instr.operation.d1BusSrc.imm); break;
                }
                Comma().OperandWrite(ymir::scu::ToString(instr.operation.d1BusDst));
                break;
            case D1BusOp::MOV_S_D:
                Mnemonic("MOV ")
                    .OperandRead(ymir::scu::ToString(instr.operation.d1BusSrc.reg))
                    .Comma()
                    .OperandWrite(ymir::scu::ToString(instr.operation.d1BusDst));
                break;
            }

            break;
        }
        case InstrType::MVI:
            Mnemonic("MVI ");
            switch (instr.mvi.dst) {
            case ymir::scu::SCUDSPInstruction::MVIDst::MC0: [[fallthrough]];
            case ymir::scu::SCUDSPInstruction::MVIDst::MC1: [[fallthrough]];
            case ymir::scu::SCUDSPInstruction::MVIDst::MC2: [[fallthrough]];
            case ymir::scu::SCUDSPInstruction::MVIDst::MC3: U32(instr.mvi.imm & 0x3F); break;
            case ymir::scu::SCUDSPInstruction::MVIDst::RX: U32(instr.mvi.imm); break;
            case ymir::scu::SCUDSPInstruction::MVIDst::P: S32(instr.mvi.imm); break;
            case ymir::scu::SCUDSPInstruction::MVIDst::WA0: [[fallthrough]];
            case ymir::scu::SCUDSPInstruction::MVIDst::RA0: U32((instr.mvi.imm << 2) & 0x7FF'FFFC); break;
            case ymir::scu::SCUDSPInstruction::MVIDst::LOP: U32(instr.mvi.imm & 0xFFF); break;
            case ymir::scu::SCUDSPInstruction::MVIDst::PC: U32(instr.mvi.imm & 0xFF); break;
            default: S32(instr.mvi.imm); break;
            }
            Comma().OperandWrite(ymir::scu::ToString(instr.mvi.dst));
            if (instr.mvi.cond != ymir::scu::SCUDSPInstruction::Cond::None) {
                Comma().Cond(instr.mvi.cond);
            }
            break;
        case InstrType::DMA:
            Mnemonic(instr.dma.hold ? "DMAH " : "DMA ");
            if (instr.dma.toD0) {
                OperandRead(ymir::scu::ToString(instr.dma.ramOp));
            } else {
                OperandRead("D0");
            }
            Comma();
            if (instr.dma.toD0) {
                OperandWrite("D0");
            } else {
                OperandWrite(ymir::scu::ToString(instr.dma.ramOp));
            }
            Comma();
            if (instr.dma.countType) {
                auto reg = fmt::format("M{}{}", (instr.dma.count.ct < 4 ? "" : "C"), instr.dma.count.ct & 3);
                OperandRead(reg);
            } else {
                U8(instr.dma.count.imm);
            }
            break;
        case InstrType::JMP:
            Mnemonic("JMP ");
            if (instr.jmp.cond != ymir::scu::SCUDSPInstruction::Cond::None) {
                Cond(instr.jmp.cond).Comma();
            }
            U8(instr.jmp.target);
            break;
        case InstrType::LPS: Mnemonic("LPS"); break;
        case InstrType::BTM: Mnemonic("BTM"); break;
        case InstrType::END: Mnemonic("END"); break;
        case InstrType::ENDI: Mnemonic("ENDI"); break;
        default: disasm.IllegalMnemonic(); break;
        }

        return *this;
    }

    SCUDSPDisassembler &Disassemble(uint32 opcode) {
        const ymir::scu::SCUDSPInstruction &instr = ymir::scu::Disassemble(opcode);

        Address().Opcode(opcode).Instruction(instr);

        disasm.NewLine();

        ++address;

        return *this;
    };

    Disassembler &disasm;
    uint8 address = 0;
};

using SCUDSPOpcode = uint32;
using SCUDSPOpcodeFetcher = IOpcodeFetcher<SCUDSPOpcode>;
using CommandLineSCUDSPOpcodeFetcher = CommandLineOpcodeFetcher<SCUDSPOpcode>;
using StreamSCUDSPOpcodeFetcher = StreamOpcodeFetcher<SCUDSPOpcode>;

bool DisassembleSCUDSP(Disassembler &disasm, std::string_view origin, const std::vector<std::string> &args,
                       const std::string &inputFile) {
    auto maybeAddress = ParseHex<uint8>(origin);
    if (!maybeAddress) {
        fmt::println("Invalid origin address: {}", origin);
        return false;
    }

    SCUDSPDisassembler scuDspDisasm{disasm};
    scuDspDisasm.address = *maybeAddress;

    auto fetcher =
        MakeFetcher<SCUDSPOpcodeFetcher, CommandLineSCUDSPOpcodeFetcher, StreamSCUDSPOpcodeFetcher>(args, inputFile);
    if (!fetcher) {
        return false;
    }

    bool running = true;
    const auto visitor = [&](auto &value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, SCUDSPOpcode>) {
            scuDspDisasm.Disassemble(value);
        } else if constexpr (std::is_same_v<T, OpcodeFetchError>) {
            fmt::println("{}", value.message);
            running = false;
        } else if constexpr (std::is_same_v<T, OpcodeFetchEnd>) {
            running = false;
        }
    };

    while (running) {
        auto result = fetcher->Fetch();
        std::visit(visitor, result);
    }

    return true;
}
