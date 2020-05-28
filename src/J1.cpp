#include <bitset>
#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>

#include "IOConsole.hpp"
#include "J1.hpp"
#include "Logger.hpp"
#include "Stack.hpp"

J1Core::J1Core() : dstack(Stack<uint16_t>(32)), rstack(Stack<uint16_t>(33)), pc(0) { std::memset(mem, 0, sizeof(mem)); }

void J1Core::Reset()
{
    pc = 0;
    dstack = Stack<uint16_t>(32);
    rstack = Stack<uint16_t>(33);
    std::memset(mem, 0, sizeof(mem));
}

int J1Core::WriteMem(uint16_t addr, uint16_t value)
{
    if (addr & (1 << 15)) {
        warn("Address $%04x is out of range!", addr);
        return -1;
    }
    if (addr & J1_MEM_IO_MASK) {
        dbg("Write IO: %04x at 0%04x", value, addr);
        IOConsole::Write(addr);
    } else {
        dbg("Write RAM: %04x at $%04x", value, addr);
        mem[addr] = value;
    }
    return 0;
}

int J1Core::ReadMem(uint16_t addr, uint16_t *value)
{
    assert(value);
    if (addr & (1 << 15)) {
        warn("Address $%04x is out of range!", addr);
        return -1;
    }
    if (addr & J1_MEM_IO_MASK) {
        dbg("Read IO $%04x", addr);
        *value = 0xbeef >> 1;
        IOConsole::Read(addr);
    } else {
        dbg("Read RAM $%04x", addr);
        *value = mem[addr];
    }
    return 0;
}

void J1Core::PrintStackDump()
{
    auto print_stack = [](Stack<uint16_t> &s) {
        if (s.Empty()) {
            std::cout << " <empty>" << std::endl;
        }
        for (auto &&vv : s.ToSeq()) {
            std::cout << " " << std::bitset<16>(vv) << std::endl;
        }
    };

    std::cout << "PC:     " << std::bitset<16>(pc) << std::endl;
    std::cout << "Data stack:" << std::endl;
    print_stack(dstack);
    std::cout << "Return stack:" << std::endl;
    print_stack(rstack);
}

void J1Core::PrintRAMDump()
{
    uint16_t empty_count = 0;
    bool ram_empty = true;
    for (int i = 0; i < J1_RAM_SIZE - 64; i = i + 64) {
        bool found_non_zero = false;
        std::stringstream ss;
        for (int j = i; j < i + 64; j = j + 2) {
            if (mem[j] || mem[j + 1]) {
                found_non_zero = true;
                ram_empty = false;
            }
            ss << std::hex << mem[j] << mem[j + 1] << " ";
        }
        if (found_non_zero) {
            if (empty_count != 0) {
                std::cout << " <" << empty_count << " empty lines>" << std::endl;
            }
            std::cout << (int)(((i + 1) / 64) + 1) << "| " << ss.str() << std::endl;
            empty_count = 0;
        } else {
            empty_count += 1;
        }
    }
    if (ram_empty) {
        std::cout << "<empty>" << std::endl;
    }
    if (empty_count != 0) {
        std::cout << " <" << empty_count << " empty lines>" << std::endl;
    }
}
