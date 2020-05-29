#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "J1.hpp"
#include "Logger.hpp"
#include "Stack.hpp"
#include "VM.hpp"

Instruction VM::parseInstructionTy(uint16_t instruction)
{
    if ((instruction >> 15) & 1U) {
        return Instruction::LIT;
    }
    switch (instruction >> 13) {
    case 0x0:
        return Instruction::JMP;
    case 0x1:
        return Instruction::CONDJMP;
    case 0x2:
        return Instruction::CALL;
    case 0x3:
        return Instruction::ALU;
    }
    return Instruction::Unknown;
}

ALUFields VM::parseAluFields(uint16_t instruction)
{
    return ALUFields{
        .T = static_cast<uint8_t>((instruction & 0x0fff) >> 8),
        .TtoN = static_cast<uint8_t>(((instruction >> 4) & 0x8) >> 3),
        .RtoPC = static_cast<uint8_t>((instruction >> 12) & 0x1),
        .TtoR = static_cast<uint8_t>(((instruction >> 4) & 0x4) >> 2),
        .dstack = static_cast<uint8_t>(instruction & 0x3),
        .rstack = static_cast<uint8_t>((instruction & 0xc) >> 2),
        .NtoT = static_cast<uint8_t>(((instruction >> 4) & 0x2) >> 1),
    };
}

uint16_t VM::parseInputExn(std::string line)
{
    if (line.length() != 16) {
        throw std::invalid_argument("Incorrect length");
    }
    return std::stoi(line, 0, 2);
}

void VM::StartInteractive()
{
    std::string line;

    std::cout << "Starting interactive session..." << std::endl;
    std::cout << "Type #help to show help information." << std::endl;

    do {
        std::cout << "> ";
        getline(std::cin, line);
        if (line.empty()) {
            continue;
        }

        // Process builtin commands
        if (line.at(0) == '#') {
            if (line == "#h" || line == "#help") {
                std::cout << "Builtin commands:" << std::endl;
                std::cout << " #sdump   Print stack dump" << std::endl;
                std::cout << " #mdump   Print RAM dump (addresses $0-$0x3fff)" << std::endl;
                std::cout << " #reset   Reset CPU" << std::endl;
                std::cout << " #exit    Close interactive session" << std::endl;
                std::cout << " #help    Print the help" << std::endl;
                std::cout << std::endl;
                std::cout << "Examples:" << std::endl;
                std::cout << std::endl;
                std::cout << "Sum two literals:" << std::endl;
                std::cout << "1000000000001" << std::endl;
                std::cout << "1000000000011" << std::endl;
                std::cout << "0110001000100" << std::endl;
                std::cout << "#m" << std::endl;
                std::cout << std::endl;
                continue;
            }
            if (line == "#s" || line == "#sdump") {
                _c.PrintStackDump();
                continue;
            }
            if (line == "#m" || line == "#mdump") {
                _c.PrintRAMDump();
                continue;
            }
            if (line == "#r" || line == "#reset") {
                _c.Reset();
                continue;
            }
            if (line == "#e" || line == "#exit") {
                return;
            }
        }

        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

        try {
            execute(parseInputExn(line));
        } catch (std::invalid_argument &) {
            err("Invalid instruction!");
        }

    } while (true);
}

int VM::ExecuteImage(const char *file_path, bool show_dump)
{
    std::ifstream input(file_path);
    std::string line;
    size_t line_count = 0;

    if (!input) {
        info("Empty ROM");
        return 0;
    }

    while (getline(input, line)) {
        line_count += 1;
        try {
            execute(parseInputExn(line));
        } catch (std::invalid_argument &) {
            err("%llu: Invalid instruction", (unsigned long long)line_count);
            return -1;
        }
    }

    info("Completed");
    if (show_dump) {
        std::cout << "RAM:" << std::endl;
        _c.PrintRAMDump();
        _c.PrintStackDump();
    }

    return 0;
}

void VM::execute(uint16_t instruction)
{
    switch (parseInstructionTy(instruction)) {
    case Instruction::LIT:
        if (_c.dstack.Push(instruction & J1_VALUE_MASK) == -1) {
            warn("Data stack overflow! Resetting CPU...");
            _c.Reset();
            break;
        }
        _c.pc += 1;
        break;
    case Instruction::JMP:
        _c.pc = instruction & J1_TARGET_MASK;
        break;
    case Instruction::CONDJMP:
        if (_c.dstack.Top().value_or(0) == 0) {
            _c.pc = instruction & J1_TARGET_MASK;
        }
        _c.dstack.Pop();
        break;
    case Instruction::CALL:
        if (_c.rstack.Push(_c.pc) == -1) {
            warn("Return stack overflow! Resetting CPU...");
            _c.Reset();
            break;
        }
        _c.pc = instruction & J1_TARGET_MASK;
        break;
    case Instruction::ALU:
        executeAlu(instruction);
        break;
    default:
        warn("Unknown instruction: %16X", instruction);
        break;
    }
}

void VM::executeAlu(uint16_t instruction)
{
    auto fields = parseAluFields(instruction);
    // Copy R to the PC
    if (fields.RtoPC && !_c.rstack.Empty()) {
        _c.pc = _c.rstack.Top().value();
    }
    // Write RAM at T
    if (fields.NtoT) {
        if (_c.dstack.Depth() < 2) {
            warn("Can't perform NtoT with data stack depth %lu", _c.dstack.Depth());
            return;
        }
        if (_c.WriteMem(_c.dstack.Top().value(), _c.dstack.Next().value()) == -1) {
            return;
        }
    }
    // Evaluate ALU command
    {
        int res;
        if ((res = evalALU(fields)) != -1) {
            if (_c.dstack.Push(res) == -1) {
                warn("Data stack overflow! Resetting CPU...");
                _c.Reset();
                return;
            }
        }
    }
    // Copy T to N
    if (fields.TtoN) {
        if (_c.dstack.SwapTN() == -1) {
            warn("Can't perform TtoN with data stack depth %lu", _c.dstack.Depth());
        }
    }
    // Copy T to R
    if (fields.TtoR) {
        if (_c.dstack.Empty()) {
            warn("Can't perform TtoR with empty data stack");
        } else {
            auto v = _c.dstack.Top().value();
            if (_c.rstack.Push(v) == -1) {
                warn("Return stack overflow! Resetting CPU...");
                _c.Reset();
                return;
            }
        }
    }
}

int VM::evalALU(ALUFields fields)
{
    auto T = _c.dstack.Top().value_or(0);
    auto N = _c.dstack.Next().value_or(0);
    auto R = _c.rstack.Top().value_or(0);

    switch (static_cast<Opcode>(fields.T)) {
    case Opcode::T:
        return T;
    case Opcode::N:
        return N;
    case Opcode::TplusN:
        return T + N;
    case Opcode::TandN:
        return T & N;
    case Opcode::TorN:
        return T | N;
    case Opcode::TxorN:
        return T ^ N;
    case Opcode::NotT:
        return ~T;
    case Opcode::NeqT:
        return (T == N) ? 1 : 0;
    case Opcode::NleT:
        return static_cast<int16_t>(N) < static_cast<int16_t>(T) ? 1 : 0;
    case Opcode::NrshiftT:
        return N >> (T & 0xf);
    case Opcode::Tminus1:
        return T - 1;
    case Opcode::R:
        return R;
    case Opcode::AtT: {
        uint16_t v;
        if (_c.ReadMem(T, &v) == -1) {
            return -1;
        }
        return v;
    }
    case Opcode::NlshiftT:
        return N << (T & 0xf);
    case Opcode::Depth:
        return (_c.rstack.Depth() << 8) | _c.dstack.Depth();
    case Opcode::NulT:
        return N < T ? 1 : 0;
    }

    warn("Unknown opcode: 0x%X", static_cast<int>(fields.T));
    return -1;
}
