#include "gtest/gtest.h"

#include "J1.hpp"
#include "VM.hpp"

class VMTest : public ::testing::Test {
  protected:
    Instruction parseInstructionTy(uint16_t instr) { return vm.parseInstructionTy(instr); }
    ALUFields parseAluFields(uint16_t instr) { return vm.parseAluFields(instr); }
    void execute(uint16_t instr) { return vm.execute(instr); }

    // Properties
    uint16_t pc() { return vm._c.pc; }
    uint16_t *mem() { return vm._c.mem; }
    Stack<uint16_t> dstack() { return vm._c.dstack; }
    Stack<uint16_t> rstack() { return vm._c.rstack; }

  private:
    VM vm;
};

TEST_F(VMTest, TestParseInstructionTy)
{
    EXPECT_EQ(parseInstructionTy(0b1000'0000'0000'0000), Instruction::LIT);
    EXPECT_EQ(parseInstructionTy(0b0000'0000'0000'0000), Instruction::JMP);
    EXPECT_EQ(parseInstructionTy(0b0010'0000'0000'0000), Instruction::CONDJMP);
    EXPECT_EQ(parseInstructionTy(0b0100'0000'0000'0000), Instruction::CALL);
    EXPECT_EQ(parseInstructionTy(0b0110'0000'0000'0000), Instruction::ALU);
}

TEST_F(VMTest, TestParseAluFields)
{
    auto fields = parseAluFields(0b0111'1100'1010'0110);
    EXPECT_EQ(fields.RtoPC, 0b1);
    EXPECT_EQ(fields.T, 0b1100);
    EXPECT_EQ(fields.TtoN, 0b1);
    EXPECT_EQ(fields.TtoR, 0b0);
    EXPECT_EQ(fields.NtoT, 0b1);
    EXPECT_EQ(fields.rstack, 0b01);
    EXPECT_EQ(fields.dstack, 0b10);

    fields = parseAluFields(0b0111'0100'1010'0110);
    EXPECT_EQ(fields.T, 0b0100);

    fields = parseAluFields(0b0111'0011'1010'0110);
    EXPECT_EQ(fields.T, 0b0011);
}

TEST_F(VMTest, TestExecutePCGrow)
{
    auto pc_init = pc();
    execute(0b1000'0000'0000'0000);
    execute(0b1000'0000'0000'0000);
    EXPECT_EQ(pc(), pc_init + 2);
}

TEST_F(VMTest, TestEvalALUT)
{
    execute(0b1000'0000'0000'0011);
    execute(0b0110'0000'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0011);
}

TEST_F(VMTest, TestEvalALUN)
{
    execute(0b1000'0000'0000'0111);
    execute(0b1000'0000'0000'0001);
    execute(0b0110'0001'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0111);
}

TEST_F(VMTest, TestEvalALUTplusN)
{
    execute(0b1000'0000'0000'0011);
    execute(0b1000'0000'0000'0010);
    execute(0b0110'0010'0010'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0101);
}

TEST_F(VMTest, TestEvalALUTandN)
{
    execute(0b1000'0000'0000'0111);
    execute(0b1000'0000'0000'1001);
    execute(0b0110'0011'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0001);
}

TEST_F(VMTest, TestEvalALUTorN)
{
    execute(0b1000'0000'0000'0111);
    execute(0b1000'0000'0000'1001);
    execute(0b0110'0100'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'1111);
}

TEST_F(VMTest, TestEvalALUTxorN)
{
    execute(0b1000'0000'0000'1100);
    execute(0b1000'0000'0000'0010);
    execute(0b0110'0101'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'1110);
}

TEST_F(VMTest, TestEvalALUNotT)
{
    execute(0b1111'1011'1111'1000);
    execute(0b0110'0110'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b1000'0100'0000'0111);
}

TEST_F(VMTest, TestEvalALUNeqT)
{
    execute(0b1111'1011'1111'1000);
    execute(0b1111'1011'1111'1000);
    execute(0b0110'0111'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0001);

    execute(0b1111'1011'1111'1010);
    execute(0b1111'1011'1111'1000);
    execute(0b0110'0111'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0000);
}

TEST_F(VMTest, TestEvalALUNleT)
{
    execute(0b1000'0000'0000'1000);
    execute(0b1000'0000'0000'1001);
    execute(0b0110'1000'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0001);

    execute(0b1000'0000'0000'1001);
    execute(0b1000'0000'0000'1000);
    execute(0b0110'1000'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0000);
}

TEST_F(VMTest, TestEvalALUNrshiftT)
{
    // 8 >> 2 == 2
    execute(0b1000'0000'0000'1000);
    execute(0b1000'0000'0000'0010);
    execute(0b0110'1001'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0010);

    // 8 >> 9 == 0
    execute(0b1000'0000'0000'1000);
    execute(0b1000'0000'0000'1001);
    execute(0b0110'1001'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0000);
}

TEST_F(VMTest, TestEvalALUTminus1)
{
    execute(0b1000'0000'0000'1000);
    execute(0b0110'1010'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0111);
}

// TEST_F(VMTest, TestEvalALUR)
// {
//     execute(0b1000'0000'0000'1000);
//     execute(0b0110'1011'0000'0000);
//     EXPECT_EQ(dstack().Top().value_or(0), 0b1000'0000'0000'0111);
// }

TEST_F(VMTest, TestEvalALUAtT)
{
    // write mem[0008]
    execute(0b1000'0000'0000'1010); // value
    execute(0b1000'0000'0000'1000); // address
    execute(0b0110'0000'0010'0000); // write
    // read mem[0008]
    execute(0b1000'0000'0000'1000); // address
    execute(0b0110'1100'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'1010);
}

TEST_F(VMTest, TestEvalALUNlshiftT)
{
    execute(0b1000'0000'0000'1000);
    execute(0b0110'1101'0000'0000);
    // EXPECT_EQ(dstack().Top().value_or(0), 0b1000'0000'0000'0111);
}

// TEST_F(VMTest, TestEvalALUNDepth)
// {
//     execute(0b1000'0000'0000'1000);
//     execute(0b0110'1101'0000'0000);
//     EXPECT_EQ(dstack().Top().value_or(0), 0b1000'0000'0000'0111);
// }

TEST_F(VMTest, TestEvalALUNultT)
{
    execute(0b1000'0000'1000'1000);
    execute(0b1000'0000'0000'1000);
    execute(0b0110'1111'0000'0000);
    EXPECT_EQ(dstack().Top().value_or(0), 0b0000'0000'0000'0000);
}

TEST_F(VMTest, TestWriteMem)
{
    // $0x2fff: Write in RAM.
    execute(0b1111'1101'1011'1111); // value
    execute(0b1010'1111'1111'1111); // address
    execute(0b0110'0000'0010'0000);
    EXPECT_EQ(mem()[0b0010'1111'1111'1111], 0b0111'1101'1011'1111);

    // $499a: IO. No write operation will performed.
    execute(0b1100'1001'1001'1010);
    execute(0b0110'0000'0010'0000);
}
