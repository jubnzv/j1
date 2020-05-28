/// Common definitions for j1 CPU.
/// See doc/j1.pdf for the reference.
#ifndef J1_HPP
#define J1_HPP

#include <cstdint>

#include "Stack.hpp"

#define J1_VALUE_MASK  0x7fff
#define J1_TARGET_MASK 0x1fff
#define J1_RAM_SIZE    0x3fff
#define J1_MEM_IO_MASK 3 << 14

/// 16-bit instruction format
enum class Instruction {
    Unknown = 0,
    LIT,
    JMP,
    CONDJMP,
    CALL,
    ALU,
};

/// ALU opcodes
enum class Opcode {
    T = 0,
    N,
    TplusN,
    TandN,
    TorN,
    TxorN,
    NotT,
    NeqT,
    NleT,
    NrshiftT,
    Tminus1,
    R,
    AtT,
    NlshiftT,
    Depth,
    NulT,
};

/// Fields of ALU instruction
/// This definition follows the convention that the top of stack is T, the
/// second item on the stack is N, and the top of the return stack is R.
struct ALUFields {
    uint8_t T : 4;      ///< ALU op, replaces T
    uint8_t TtoN : 1;   ///< Copy T to N
    uint8_t RtoPC : 1;  ///< Copy R to PC
    uint8_t TtoR : 1;   ///< Copy T to R
    uint8_t dstack : 2; ///< Signed increment data stack
    uint8_t rstack : 2; ///< Signed increment return stack
    uint8_t NtoT : 1;   ///< RAM write
};

/// Internal state of J1 CPU.
class J1Core {
  public:
    J1Core();

    /// Write memory at the given address
    /// @return 0 if success, -1 if address is out of range
    int WriteMem(uint16_t addr, uint16_t value);

    /// Read memory at the given address
    /// @param[out] value RAM value
    /// @return 0 if success, -1 if address is out of range
    int ReadMem(uint16_t addr, uint16_t *value);

    /// Reset CPU
    void Reset();

    /// Print dumps of return and data stacks
    void PrintStackDump();

    /// Print dump of RAM
    void PrintRAMDump();

  public:
    /// Data stack
    Stack<uint16_t> dstack;
    /// Return stack
    Stack<uint16_t> rstack;
    /// Program counter
    uint16_t pc : 13;
    /// Memory representation
    ///  $0-$0x3fff: RAM used for code and data
    ///  $4000-$0x7fff: memory-mapped IO
    /// We will store only RAM values here.
    uint16_t mem[J1_RAM_SIZE];
};

#endif // J1_HPP
