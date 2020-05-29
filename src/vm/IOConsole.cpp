#include <bitset>
#include <cstdint>
#include <iostream>

#include "IOConsole.hpp"
#include "Term.hpp"

static std::string get_ioc(uint16_t n)
{
    auto s = std::bitset<16>(n).to_string();
    for (auto &c : s) {
        if (c == '0') {
            c = '-';
        } else {
            c = '+';
        }
    }
    return s;
}

void IOConsole::Write(uint16_t addr)
{
    (void)addr; // do nothing
}

void IOConsole::Read(uint16_t addr)
{
    std::cout << TCMAP.at(TC::GREEN) << get_ioc(addr >> 15) << TCMAP.at(TC::ENDC) << std::endl;
}
