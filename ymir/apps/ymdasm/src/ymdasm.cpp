#include "disassembler_m68k.hpp"
#include "disassembler_scspdsp.hpp"
#include "disassembler_scudsp.hpp"
#include "disassembler_sh2.hpp"

#include "colors.hpp"

#include <cxxopts.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    bool showHelp = false;
    bool hideAddresses = false;
    bool hideOpcodes = false;
    std::string colorMode = "none";
    std::string inputFile{};
    std::string origin{};

    std::string isa{};
    std::vector<std::string> args{};

    cxxopts::Options options("ymdasm", "Ymir disassembly tool\nVersion " Ymir_VERSION);
    options.add_options()("h,help", "Display this help text.", cxxopts::value(showHelp)->default_value("false"));
    options.add_options()("C,color", "Color text output (stdout only): none, basic, truecolor",
                          cxxopts::value(colorMode)->default_value("none"), "color_mode");
    options.add_options()("i,input-file",
                          "Disassemble code from the specified file. Omit to disassemble command line arguments.",
                          cxxopts::value(inputFile), "path");
    options.add_options()("o,origin", "Origin (base) address of the disassembled code.",
                          cxxopts::value(origin)->default_value("0"), "address");
    options.add_options()("a,hide-addresses", "Hide addresses from disassembly listing.",
                          cxxopts::value(hideAddresses)->default_value("false"));
    options.add_options()("c,hide-opcodes", "Hide opcodes from disassembly listing.",
                          cxxopts::value(hideOpcodes)->default_value("false"));

    options.add_options()("isa", "Instruction set architecture: sh2, sh-2, m68k, m68000, scudsp, scspdsp",
                          cxxopts::value(isa));
    options.add_options()("opcodes", "Sequence of program opcodes", cxxopts::value(args));

    options.parse_positional({"isa", "opcodes"});
    options.positional_help("<isa> {<program opcodes>|[<offset> [<length>]]}");

    auto printHelp = [&] {
        fmt::println("{}", options.help());
        fmt::println("  <isa> specifies an instruction set architecture to disassemble:");
        fmt::println("    sh2, sh-2     Hitachi/Renesas SuperH-2");
        fmt::println("    m68k, m68000  Motorola 68000");
        fmt::println("    scudsp        SCU (Saturn Control Unit) DSP");
        fmt::println("    scspdsp       SCSP (Saturn Custom Sound Processor) DSP");
        fmt::println("    scspdspraw    SCSP (Saturn Custom Sound Processor) DSP (raw dissasembly)");
        fmt::println("  This argument is case-insensitive.");
        fmt::println("");
        fmt::println("  When disassembling command line arguments, <program opcodes> specifies the");
        fmt::println("  hexadecimal opcodes to disassemble.");
        fmt::println("");
        fmt::println("  When disassembling from a file, <offset> specifies the offset from the start");
        fmt::println("  of the file and <length> determines the number of bytes to disassemble.");
        fmt::println("  Both parameters are specified in hexadecimal.");
        fmt::println("  <length> is truncated down to the nearest multiple of the opcode size.");
        fmt::println("  If <offset> is omitted, ymdasm disassembles from the start of the file.");
        fmt::println("  If <length> is omitted, ymdasm disassembles until the end of the file.");
        fmt::println("");
        fmt::println("  SuperH-2 opcodes can be prefixed with ! to force them to be decoded as delay");
        fmt::println("  slot instructions or _ to force instructions in delay slots to be decoded as");
        fmt::println("  regular instructions.");
    };

    try {
        auto result = options.parse(argc, argv);

        // Show help if requested
        if (showHelp) {
            printHelp();
            return 0;
        }

        // ISA is required
        if (!result.contains("isa")) {
            fmt::println("Missing argument: <isa>");
            fmt::println("");
            printHelp();
            return 1;
        }

        // Must specify at least one opcode when disassembling from command line
        if (inputFile.empty() && args.empty()) {
            fmt::println("Missing argument: <program opcodes>");
            fmt::println("");
            printHelp();
            return 1;
        }

        Disassembler disasm{};
        disasm.hideAddresses = hideAddresses;
        disasm.hideOpcodes = hideOpcodes;

        // Color mode must be one of the valid modes
        if (result.contains("color")) {
            std::string lcColorMode = colorMode;
            std::transform(lcColorMode.cbegin(), lcColorMode.cend(), lcColorMode.begin(),
                           [](char c) { return std::tolower(c); });
            if (lcColorMode == "none") {
                disasm.colors = kNoColors;
            } else if (lcColorMode == "basic") {
                disasm.colors = kBasicColors;
            } else if (lcColorMode == "truecolor") {
                disasm.colors = kTrueColors;
            } else {
                fmt::println("Invalid color mode: {}", colorMode);
                fmt::println("");
                printHelp();
                return 1;
            }
        }

        // Disassemble code
        std::string lcisa = isa;
        std::transform(lcisa.cbegin(), lcisa.cend(), lcisa.begin(), [](char c) { return std::tolower(c); });
        if (lcisa == "sh2" || lcisa == "sh-2") {
            if (!DisassembleSH2(disasm, origin, args, inputFile)) {
                return 1;
            }
        } else if (lcisa == "m68k" || lcisa == "m68000") {
            if (!DisassembleM68K(disasm, origin, args, inputFile)) {
                return 1;
            }
        } else if (lcisa == "scudsp") {
            if (!DisassembleSCUDSP(disasm, origin, args, inputFile)) {
                return 1;
            }
        } else if (lcisa == "scspdspraw" || lcisa == "scspdsp") {
            const bool raw = lcisa == "scspdspraw";
            if (!DisassembleSCSPDSP(disasm, origin, args, inputFile, raw)) {
                return 1;
            }
        } else {
            fmt::println("Invalid ISA: {}", isa);
            fmt::println("");
            printHelp();
            return 1;
        }
    } catch (const cxxopts::exceptions::exception &e) {
        fmt::println("Failed to parse arguments: {}", e.what());
        return -1;
    } catch (const std::system_error &e) {
        fmt::println("System error: {}", e.what());
        return e.code().value();
    } catch (const std::exception &e) {
        fmt::println("Unhandled exception: {}", e.what());
        return -1;
    }
}
