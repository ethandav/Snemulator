#include "pch.h"
#include "gtest/gtest.h"
#include "src/cpu.h"
#include "src/memory.h"

class CPUOpcodeTest : public ::testing::Test
{
protected:
    std::unique_ptr<RAM> ram;
    std::unique_ptr<ROM> rom;
    std::unique_ptr<MemoryMap> memory;
    CPU cpu;

    void LoadProgram(const std::vector<uint8_t>& program)
    {
        rom = std::make_unique<ROM>(program);
        memory->map(0x008000, 0x008000 + rom.get()->size() - 1, rom.get());
    }

    void SetUp() override
    {
        ram = std::make_unique<RAM>(0x20000);
        memory = std::make_unique<MemoryMap>();
        memory->map(0x7E0000, 0x7FFFFF, ram.get());
        cpu.memory = memory.get();
        cpu.registers.PC = 0x008000;
        cpu.registers.DBR = 0x7E;
        cpu.running = true;
        register_opcodes(cpu);
    }
};

TEST_F(CPUOpcodeTest, LDA_Immediate_LoadsCorrectValue)
{
    LoadProgram({
        0xA9, 0x42,
        0x00
    });

    cpu.run();

    EXPECT_EQ(cpu.registers.A, 0x42);
}

TEST_F(CPUOpcodeTest, LDA_Absolute_LoadsCorrectValue)
{
    LoadProgram({
        0xAD, 0x42,
        0x00
        });

    cpu.run();

    EXPECT_EQ(cpu.registers.A, 0x42);
}

TEST_F(CPUOpcodeTest, LDA_Indexed_X_LoadsCorrectValue)
{
    LoadProgram({
        0xBD, 0x42,
        0x00
        });

    cpu.run();

    EXPECT_EQ(cpu.registers.A, 0x42);
}

TEST_F(CPUOpcodeTest, LDX_Indexed_Y_LoadsCorrectValue)
{
    LoadProgram({
        0xBD, 0x42,
        0x00
        });

    cpu.run();

    EXPECT_EQ(cpu.registers.A, 0x42);
}