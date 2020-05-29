#ifndef VM_HPP
#define VM_HPP

#include <cstdint>

#include "J1.hpp"

class VM {
    friend class VMTest;

  public:
    VM() {}

    /// Start interactive session that works like REPL
    void StartInteractive();
    /// Execute given image file with CPU bytecode
    /// @return 0 if successfully executed, -1 on errors
    int ExecuteImage(const char *file_path, bool show_dump);

  private:
    /// Execute given instruction
    void execute(uint16_t);

  private:
    /// Parse type of a given instruction
    Instruction parseInstructionTy(uint16_t);
    /// Parse fields of ALU instruction
    ALUFields parseAluFields(uint16_t);
    /// Parse user input
    /// @return Instruction value
    uint16_t parseInputExn(std::string line);
    /// Execute ALU command
    void executeAlu(uint16_t);
    /// Evaluate ALU command
    int evalALU(ALUFields);

  private:
    J1Core _c; /// J1's internal state
};

#endif // VM_HPP
