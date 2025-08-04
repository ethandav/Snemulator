#include "pch.h"
#include "gtest/gtest.h"
#include "src/cpu.h"
#include "src/memory.h"


class CPUOpcodeTest : public ::testing::Test
{
protected:
    std::unique_ptr<Memory> memory;
    CPU cpu;

    void LoadProgram(const std::vector<uint8_t>& program)
    {
        memory->loadProgram(program);
    }

    void SetUp() override
    {
        memory = std::make_unique<Memory>();
        memory->init();
        cpu.memory = &memory.get()->map;
        cpu.registers.PC = 0x008000;
        cpu.registers.DBR = 0x7E;
        cpu.running = true;
        register_opcodes(cpu);
    }
};
namespace OpCode_Tests
{
    namespace LDA_Tests
    {
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
            memory->wram.write(0x1234, 0x44 & 0xFF);
            LoadProgram({
                0xAD, 0x34, 0x12,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x44);
        }
        TEST_F(CPUOpcodeTest, LDA_Absolute_Indexed_X_LoadsCorrectValue)
        {
            memory->wram.write(0x1239, 0x23 & 0xFF);
            cpu.registers.X = 0x05;
            LoadProgram({
                0xBD, 0x34, 0x12,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x23);
        }
        TEST_F(CPUOpcodeTest, LDA_Absolute_Indexed_Y_LoadsCorrectValue)
        {
            memory->wram.write(0x1239, 0x25 & 0xFF);
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
            memory->wram.write(0x0010, 0x26 & 0xFF);
            LoadProgram({
                0xA5, 0x10,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x26);
        }
        TEST_F(CPUOpcodeTest, LDA_DirectPage_LoadsCorrectValue)
        {
            memory->wram.write(0x0020, 0x42 & 0xFF);
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
            memory->wram.write(0x0025, 0x42 & 0xFF);
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
            memory->wram.write(0x1234, 0x26 & 0xFF);
            memory->wram.write(0x0010, 0x34 & 0xFF);
            memory->wram.write(0x0011, 0x12 & 0xFF);
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
            memory->wram.write(0x1234, 0x31 & 0xFF);
            memory->wram.write(0x0020, 0x34 & 0xFF);
            memory->wram.write(0x0021, 0x12 & 0xFF);
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
            memory->wram.write(0x1244, 0x17 & 0xFF);
            memory->wram.write(0x0010, 0x34 & 0xFF);
            memory->wram.write(0x0011, 0x12 & 0xFF);
            cpu.registers.D = 0x0000;
            cpu.registers.Y = 0x10;
            LoadProgram({
                0xB1, 0x10,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x17);
        }
        TEST_F(CPUOpcodeTest, LDA_AbsoluteLong_LoadsCorrectValue)
        {
            memory->wram.write(0x01234, 0x99 & 0xFF);
            LoadProgram({
                0xAF, 0x34, 0x12, 0x7E,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x99);
        }
        TEST_F(CPUOpcodeTest, LDA_DirectPage_Indirect_Long_Y_LoadsCorrectValue)
        {
            memory->wram.write(0x0012, 0x34 & 0xFF);
            memory->wram.write(0x0013, 0x12 & 0xFF);
            memory->wram.write(0x0014, 0x7E & 0xFF);
            memory->wram.write(0x01237, 0x56 & 0xFF);
            cpu.registers.D = 0x0002;
            cpu.registers.Y = 0x03;
            LoadProgram({
                0xB7, 0x10,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x56);
        }
        TEST_F(CPUOpcodeTest, LDA_DirectPage_Indirect_Long_LoadsCorrectValue)
        {
            memory->wram.write(0x0012, 0x34 & 0xFF);
            memory->wram.write(0x0013, 0x12 & 0xFF);
            memory->wram.write(0x0014, 0x7E & 0xFF);
            memory->wram.write(0x01234, 0x72 & 0xFF);
            cpu.registers.D = 0x0002;
            LoadProgram({
                0xA7, 0x10,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x72);
        }
        TEST_F(CPUOpcodeTest, LDA_StackRelative_LoadsCorrectValue)
        {
            cpu.registers.S = 0x10;
            memory->wram.write(0x0115, 0x5A);
            LoadProgram({
                0xA3, 0x05,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x5A);
        }
        TEST_F(CPUOpcodeTest, LDA_StackRelative_IndirectIndexedY_LoadsCorrectValue)
        {
            cpu.registers.S = 0x10;
            memory->wram.write(0x0115, 0x30);
            memory->wram.write(0x0116, 0x12);
            cpu.registers.Y = 0x04;
            memory->wram.write(0x01234, 0x7E);
            LoadProgram({
                0xB3, 0x05,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x7E);
        }
    }

    namespace LDX_Tests
    {
        TEST_F(CPUOpcodeTest, LDX_Immediate_LoadsCorrectValue)
        {
            LoadProgram({
                0xA2, 0x42,
                0x00
            });

            cpu.run();

            EXPECT_EQ(cpu.registers.X, 0x42);
        }
        TEST_F(CPUOpcodeTest, LDX_Absolute_LoadsCorrectValue)
        {
            memory->wram.write(0x1234, 0x44 & 0xFF);
            LoadProgram({
                0xAE, 0x34, 0x12,
                0x00
            });

            cpu.run();

            EXPECT_EQ(cpu.registers.X, 0x44);
        }
        TEST_F(CPUOpcodeTest, LDX_ZeroPage_LoadsCorrectValue)
        {
            memory->wram.write(0x0010, 0x26 & 0xFF);
            LoadProgram({
                0xA6, 0x10,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.X, 0x26);
        }
        TEST_F(CPUOpcodeTest, LDX_DirectPage_LoadsCorrectValue)
        {
            memory->wram.write(0x0020, 0x32 & 0xFF);
            cpu.registers.D = 0x10;
            LoadProgram({
                0xA6, 0x10,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.X, 0x32);
        }
        TEST_F(CPUOpcodeTest, LDX_DP_Indexed_Y_LoadsCorrectValue)
        {
            memory->wram.write(0x0025, 0x42 & 0xFF);
            cpu.registers.D = 0x10;
            cpu.registers.Y = 0x05;
            LoadProgram({
                0xB6, 0x10,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.X, 0x42);
        }
        TEST_F(CPUOpcodeTest, LDX_Absolute_Indexed_Y_LoadsCorrectValue)
        {
            memory->wram.write(0x1239, 0x25 & 0xFF);
            cpu.registers.Y = 0x05;
            LoadProgram({
                0xBE, 0x34, 0x12,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.X, 0x25);
        }
    }
}