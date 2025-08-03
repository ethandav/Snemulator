#include "pch.h"
#include "gtest/gtest.h"
#include "src/cpu.h"
#include "src/memory.h"

namespace OpCodeTests
{
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
        ram->write(0x1234, 0x44 & 0xFF);
        LoadProgram({
            0xAD, 0x34, 0x12,
            0x00
            });

        cpu.run();

        EXPECT_EQ(cpu.registers.A, 0x44);
    }
    TEST_F(CPUOpcodeTest, LDA_Indexed_X_LoadsCorrectValue)
    {
        ram->write(0x1239, 0x23 & 0xFF);
        cpu.registers.X = 0x05;
        LoadProgram({
            0xBD, 0x34, 0x12,
            0x00
            });

        cpu.run();

        EXPECT_EQ(cpu.registers.A, 0x23);
    }
    TEST_F(CPUOpcodeTest, LDA_Indexed_Y_LoadsCorrectValue)
    {
        ram->write(0x1239, 0x25 & 0xFF);
        cpu.registers.Y = 0x05;
        LoadProgram({
            0xB9, 0x34, 0x12,
            0x00
            });

        cpu.run();

        EXPECT_EQ(cpu.registers.A, 0x25);
    }
    TEST_F(CPUOpcodeTest, LDA_ZeroPage_LoadsCorrectValue)
    {
        ram->write(0x0010, 0x26 & 0xFF);
        LoadProgram({
            0xA5, 0x10,
            0x00
            });

        cpu.run();

        EXPECT_EQ(cpu.registers.A, 0x26);
    }
    TEST_F(CPUOpcodeTest, LDA_DirectPage_LoadsCorrectValue)
    {
        ram->write(0x0020, 0x42 & 0xFF);
        cpu.registers.D = 0x10;
        LoadProgram({
            0xA5, 0x10,
            0x00
            });

        cpu.run();

        EXPECT_EQ(cpu.registers.A, 0x42);
    }
    TEST_F(CPUOpcodeTest, LDA_DirectPage_X_LoadsCorrectValue)
    {
        ram->write(0x0025, 0x42 & 0xFF);
        cpu.registers.D = 0x10;
        cpu.registers.X = 0x05;
        LoadProgram({
            0xB5, 0x10,
            0x00
            });

        cpu.run();

        EXPECT_EQ(cpu.registers.A, 0x42);
    }
    TEST_F(CPUOpcodeTest, LDA_DP_Indirect_LoadsCorrectValue)
    {
        ram->write(0x1234, 0x26 & 0xFF);
        ram->write(0x0010, 0x34 & 0xFF);
        ram->write(0x0011, 0x12 & 0xFF);
        cpu.registers.D = 0x0000;
        LoadProgram({
            0xB2, 0x10,
            0x00
            });

        cpu.run();

        EXPECT_EQ(cpu.registers.A, 0x26);
    }
    TEST_F(CPUOpcodeTest, LDA_DP_Indirect_X_LoadsCorrectValue)
    {
        ram->write(0x1234, 0x31 & 0xFF);
        ram->write(0x0020, 0x34 & 0xFF);
        ram->write(0x0021, 0x12 & 0xFF);
        cpu.registers.D = 0x0000;
        cpu.registers.X = 0x10;
        LoadProgram({
            0xA1, 0x10,
            0x00
            });

        cpu.run();

        EXPECT_EQ(cpu.registers.A, 0x31);
    }
    TEST_F(CPUOpcodeTest, LDA_CP_Indirect_Y_LoadsCorrectValue)
    {
        ram->write(0x1234, 0x17 & 0xFF);
        ram->write(0x0020, 0x34 & 0xFF);
        ram->write(0x0021, 0x12 & 0xFF);
        cpu.registers.D = 0x0000;
        cpu.registers.Y = 0x10;
        LoadProgram({
            0xB1, 0x10,
            0x00
            });

        cpu.run();

        EXPECT_EQ(cpu.registers.A, 0x17);
    }
}