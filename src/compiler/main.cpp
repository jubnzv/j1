/// Toy Forth to bytecode translator for J1VM
#include <algorithm>
#include <bitset>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

// See doc/j1.pdf Table 3
#define J_FORTH_KW_DUP    0x6084
#define J_FORTH_KW_OVER   0x6184
#define J_FORTH_KW_INVERT 0x6500
#define J_FORTH_KW_PLUS   0x620c
#define J_FORTH_KW_SWAP   0x6100
#define J_FORTH_KW_NIP    0x600c
#define J_FORTH_KW_DROP   0x610c
#define J_FORTH_KW_SEMI   0x7003
#define J_FORTH_KW_GTR    0x614d
#define J_FORTH_KW_RGT    0x6b4d
#define J_FORTH_KW_RAT    0x6be4
#define J_FORTH_KW_AT     0x6c00
#define J_FORTH_KW_EXCL   0x612c

constexpr unsigned int str2int(const char *str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

static bool readDigits(std::string word, uint16_t *value)
{
    try {
        uint64_t res = std::stoi(word);
        if (res > UINT16_MAX) {
            return false;
        }
        *value = static_cast<uint16_t>(res) | 0x8000;
    } catch (std::invalid_argument &) {
        return false;
    }
    return true;
}

static std::string processWord(std::string word)
{
    uint16_t bc = 0;

    switch (str2int(word.c_str())) {
    case str2int("dup"):
        bc |= J_FORTH_KW_DUP;
        break;
    case str2int("over"):
        bc |= J_FORTH_KW_OVER;
        break;
    case str2int("invert"):
        bc |= J_FORTH_KW_INVERT;
        break;
    case str2int("+"):
        bc |= J_FORTH_KW_PLUS;
        break;
    case str2int("swap"):
        bc |= J_FORTH_KW_SWAP;
        break;
    case str2int("nip"):
        bc |= J_FORTH_KW_NIP;
        break;
    case str2int("drop"):
        bc |= J_FORTH_KW_DROP;
        break;
    case str2int(";"):
        bc |= J_FORTH_KW_SEMI;
        break;
    case str2int(">r"):
        bc |= J_FORTH_KW_GTR;
        break;
    case str2int("r>"):
        bc |= J_FORTH_KW_RGT;
        break;
    case str2int("r@"):
        bc |= J_FORTH_KW_RAT;
        break;
    case str2int("@"):
        bc |= J_FORTH_KW_AT;
        break;
    case str2int("!"):
        bc |= J_FORTH_KW_EXCL;
        break;
    default:
        uint16_t value = 0;
        if (!readDigits(word, &value)) {
            std::cerr << "Unknown word: " << word << std::endl;
        } else {
            bc = value;
        }
        break;
    }

    return std::bitset<16>(bc).to_string();
}

static std::string fromString(std::string code)
{
    std::vector<std::string> opcodes;
    std::stringstream ss(code);
    std::string word;

    while (std::getline(ss, word, ' ')) {
        opcodes.push_back(processWord(word));
    }

    if (opcodes.empty()) {
        return "";
    }

    return std::accumulate(std::next(opcodes.begin()), opcodes.end(), opcodes.at(0),
        [](std::string l, std::string r) { return l + '\n' + r; });
}

char *getCmdOption(char **begin, char **end, const std::string &option)
{
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

static void print_usage(const char *argv0)
{
    std::cout << "Usage: " << argv0 << " filename.ft [-o <rom_name>]" << std::endl;
}

int main(int argc, char *argv[])
{
    if ((argc < 2)) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    std::string rom_content = "";
    {
        std::string forth_code = "";
        try {
            std::fstream is(argv[1]);
            forth_code = std::string(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());
            forth_code.erase(std::remove(forth_code.begin(), forth_code.end(), '\n'), forth_code.end());
        } catch (...) {
            std::cerr << "There are some problems with " << argv[1] << std::endl;
            exit(EXIT_FAILURE);
        }
        rom_content = fromString(forth_code);
    }

    if (cmdOptionExists(argv, argv + argc, "-o")) {
        char *fpath = getCmdOption(argv, argv + argc, "-o");
        try {
            std::ofstream os(fpath);
            os << rom_content << std::endl;
        } catch (...) {
            std::cerr << "There are some problems with " << fpath << std::endl;
            exit(EXIT_FAILURE);
        }
    } else {
        std::cout << rom_content << std::endl;
    }

    return 0;
}
