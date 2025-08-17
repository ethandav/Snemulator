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
        TEST_F(CPUOpcodeTest, LDA_StackRelative_LoadsCorrectValue)
        {
            cpu.registers.E = 1;
            cpu.registers.S = 0x10;
            memory->wram.write(0x0115, 0x5A);
            LoadProgram({
                0xA3, 0x05,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x5A);
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
        TEST_F(CPUOpcodeTest, LDA_DP_Indirect_Y_LoadsCorrectValue)
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
        TEST_F(CPUOpcodeTest, LDA_StackRelative_IndirectIndexedY_LoadsCorrectValue)
        {
            cpu.registers.S = 0x10;
            cpu.registers.E = 1;
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
        TEST_F(CPUOpcodeTest, LDA_StackRelative_IndirectIndexedY_WrapsCorrectly)
        {
            cpu.registers.S = 0x00;
            cpu.registers.E = 1;
            memory->wram.write(0x01FF, 0x34);
            memory->wram.write(0x0100, 0x12);
            cpu.registers.Y = 0x01;
            memory->wram.write(0x01235, 0xAB);
            LoadProgram({
                0xB3, 0xFF,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0xAB);
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
        TEST_F(CPUOpcodeTest, LDA_AbsoluteLong_X_LoadsCorrectValue)
        {
            memory->wram.write(0x3919, 0x9B & 0xFF);
            cpu.registers.A = 0x9D;
            cpu.registers.X = 0x2C;
            LoadProgram({
                0xBF, 0xED, 0x38, 0x7E,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.A, 0x9B);
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
    namespace LDY_Tests
    {
        TEST_F(CPUOpcodeTest, LDY_Immediate_LoadsCorrectValue)
        {
            LoadProgram({
                0xA0, 0x7C,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.Y, 0x7C);
        }
        TEST_F(CPUOpcodeTest, LDY_Absolute_LoadsCorrectValue)
        {
            memory->wram.write(0x3456, 0x2D & 0xFF);
            LoadProgram({
                0xAC, 0x56, 0x34,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.Y, 0x2D);
        }
        TEST_F(CPUOpcodeTest, LDY_ZeroPage_LoadsCorrectValue)
        {
            memory->wram.write(0x0023, 0x4B & 0xFF);
            LoadProgram({
                0xA4, 0x23,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.Y, 0x4B);
        }
        TEST_F(CPUOpcodeTest, LDY_DirectPage_LoadsCorrectValue)
        {
            memory->wram.write(0x0024, 0x27 & 0xFF);
            cpu.registers.D = 0x12;
            LoadProgram({
                0xA4, 0x12,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.Y, 0x27);
        }
        TEST_F(CPUOpcodeTest, LDY_DP_Indexed_X_LoadsCorrectValue)
        {
            memory->wram.write(0x0014, 0x42 & 0xFF);
            cpu.registers.D = 0x09;
            cpu.registers.X = 0x02;
            LoadProgram({
                0xB4, 0x09,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.Y, 0x42);
        }
        TEST_F(CPUOpcodeTest, LDY_Absolute_Indexed_X_LoadsCorrectValue)
        {
            memory->wram.write(0x4569, 0xC3 & 0xFF);
            cpu.registers.X = 0x02;
            LoadProgram({
                0xBC, 0x67, 0x45,
                0x00
                });

            cpu.run();

            EXPECT_EQ(cpu.registers.Y, 0xC3);
        }
    }
    namespace STA_Tests
    {
        TEST_F(CPUOpcodeTest, STA_DP_Indexed_Indirect_X_LoadsCorrectValue)
        {
            memory->wram.write(0x0042, 0x54 & 0xFF);
            memory->wram.write(0x0043, 0x76 & 0xFF);
            cpu.registers.D = 0x00;
            cpu.registers.X = 0x32;
            cpu.registers.A = 0x3F;
            LoadProgram({
                0x81, 0x10,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x7654);

            EXPECT_EQ(result, 0x3F);
        }
        TEST_F(CPUOpcodeTest, STA_StackRelative_LoadsCorrectValue)
        {
            cpu.registers.S = 0x10;
            cpu.registers.A = 0x9D;
            LoadProgram({
                0x83, 0x06,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x0116);

            EXPECT_EQ(cpu.registers.A, 0x9D);
        }
        TEST_F(CPUOpcodeTest, STA_DirectPage_LoadsCorrectValue)
        {
            cpu.registers.A = 0xCD;
            cpu.registers.D = 0x10;
            LoadProgram({
                0x85, 0x10,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x0020);

            EXPECT_EQ(result, 0xCD);
        }
        TEST_F(CPUOpcodeTest, STA_DirectPage_Indirect_Long_LoadsCorrectValue)
        {
            memory->wram.write(0x0012, 0x34 & 0xFF);
            memory->wram.write(0x0013, 0x12 & 0xFF);
            memory->wram.write(0x0014, 0x7E & 0xFF);
            cpu.registers.A = 0x72;
            cpu.registers.D = 0x0002;
            LoadProgram({
                0x87, 0x10,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x01234);

            EXPECT_EQ(result, 0x72);
        }
        TEST_F(CPUOpcodeTest, STA_Absolute_LoadsCorrectValue)
        {
            cpu.registers.A = 0xE2;
            LoadProgram({
                0x8D, 0x33, 0xC1,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0xC133);

            EXPECT_EQ(result, 0xE2);
        }
        TEST_F(CPUOpcodeTest, STA_AbsoluteLong_LoadsCorrectValue)
        {
            cpu.registers.A = 0x99;
            LoadProgram({
                0x8F, 0xF2, 0x82, 0x7E,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x82F2);

            EXPECT_EQ(result, 0x99);
        }
        TEST_F(CPUOpcodeTest, STA_DP_Indirect_Y_LoadsCorrectValue)
        {
            memory->wram.write(0x0010, 0x34 & 0xFF);
            memory->wram.write(0x0011, 0x12 & 0xFF);
            cpu.registers.A = 0x17;
            cpu.registers.D = 0x0000;
            cpu.registers.Y = 0x10;
            LoadProgram({
                0x91, 0x10,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x1244);

            EXPECT_EQ(result, 0x17);
        }
        TEST_F(CPUOpcodeTest, STA_DP_Indirect_LoadsCorrectValue)
        {
            cpu.registers.A = 0xE6;
            memory->wram.write(0x0010, 0x34 & 0xFF);
            memory->wram.write(0x0011, 0x12 & 0xFF);
            cpu.registers.D = 0x0000;
            LoadProgram({
                0x92, 0x10,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x1234);

            EXPECT_EQ(result, 0xE6);
        }
        TEST_F(CPUOpcodeTest, STA_StackRelative_IndirectIndexedY_LoadsCorrectValue)
        {
            memory->wram.write(0x0115, 0x30);
            memory->wram.write(0x0116, 0x12);
            cpu.registers.S = 0x10;
            cpu.registers.Y = 0x04;
            cpu.registers.A = 0x2B;
            LoadProgram({
                0x93, 0x05,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x1234);

            EXPECT_EQ(cpu.registers.A, 0x2B);
        }
        TEST_F(CPUOpcodeTest, STA_DirectPage_X_LoadsCorrectValue)
        {
            cpu.registers.A = 0xCD;
            cpu.registers.D = 0x02;
            cpu.registers.X = 0xE1;
            LoadProgram({
                0x95, 0x10,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x00F3);

            EXPECT_EQ(result, 0xCD);
        }
        TEST_F(CPUOpcodeTest, STA_DirectPage_Indirect_Long_Y_LoadsCorrectValue)
        {
            memory->wram.write(0x0012, 0x34 & 0xFF);
            memory->wram.write(0x0013, 0x12 & 0xFF);
            memory->wram.write(0x0014, 0x7E & 0xFF);
            cpu.registers.A = 0x5A;
            cpu.registers.D = 0x0002;
            cpu.registers.Y = 0x03;
            LoadProgram({
                0x97, 0x10,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x1237);

            EXPECT_EQ(result, 0x5A);
        }
        TEST_F(CPUOpcodeTest, STA_Absolute_Indexed_Y_LoadsCorrectValue)
        {
            cpu.registers.A = 0x5E;
            cpu.registers.Y = 0x10;
            LoadProgram({
                0x99, 0x64, 0xEC,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0xEC74);

            EXPECT_EQ(result, 0x5E);
        }
        TEST_F(CPUOpcodeTest, STA_Absolute_Indexed_X_LoadsCorrectValue)
        {
            cpu.registers.A = 0x3A;
            cpu.registers.X = 0x19;
            LoadProgram({
                0x9D, 0xF1, 0x11,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x120A);

            EXPECT_EQ(result, 0x3A);
        }
        TEST_F(CPUOpcodeTest, STA_AbsoluteLong_X_LoadsCorrectValue)
        {
            cpu.registers.A = 0x9D;
            cpu.registers.X = 0xE1;
            LoadProgram({
                0x9F, 0x3D, 0x4D, 0x7E,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x4E1E);

            EXPECT_EQ(result, 0x9D);
        }
    }
    namespace STX_Tests
    {
        TEST_F(CPUOpcodeTest, STX_DirectPage_LoadsCorrectValue)
        {
            cpu.registers.X = 0xCD;
            cpu.registers.D = 0x10;
            LoadProgram({
                0x86, 0x10,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x0020);

            EXPECT_EQ(result, 0xCD);
        }
        TEST_F(CPUOpcodeTest, STX_Absolute_LoadsCorrectValue)
        {
            cpu.registers.X = 0xDC;
            LoadProgram({
                0x8E, 0x10, 0x21,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x2110);

            EXPECT_EQ(result, 0xDC);
        }
        TEST_F(CPUOpcodeTest, STX_DirectPage_Indexed_Y_LoadsCorrectValue)
        {
            cpu.registers.X = 0xCD;
            cpu.registers.D = 0xEE;
            cpu.registers.Y = 0xA4;
            LoadProgram({
                0x96, 0x4A,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x01DC);

            EXPECT_EQ(result, 0xCD);
        }
    }
    namespace STY_Tests
    {
        TEST_F(CPUOpcodeTest, STY_DirectPage_LoadsCorrectValue)
        {
            cpu.registers.Y = 0x2B;
            cpu.registers.D = 0x5D;
            LoadProgram({
                0x84, 0x02,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x005F);

            EXPECT_EQ(result, 0x2B);
        }
        TEST_F(CPUOpcodeTest, STY_Absolute_LoadsCorrectValue)
        {
            cpu.registers.Y = 0xBA;
            LoadProgram({
                0x8C, 0x1C, 0x4F,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x4F1C);

            EXPECT_EQ(result, 0xBA);
        }
        TEST_F(CPUOpcodeTest, STY_DirectPage_Indexed_X_LoadsCorrectValue)
        {
            cpu.registers.Y = 0x1F;
            cpu.registers.D = 0x2C;
            cpu.registers.X = 0x0A;
            LoadProgram({
                0x94, 0x11,
                0x00
                });

            cpu.run();
            uint16_t result = memory->wram.read(0x0047);

            EXPECT_EQ(result, 0x1F);
        }
    }
    namespace REP_SEP_Tests
    {
        TEST_F(CPUOpcodeTest, REP_SetAccumulator_16BitMode_SetsCorrectPFlags)
        {
            cpu.setFlag(M, true);
            cpu.setFlag(X, true);
            LoadProgram({
                0xC2, 0x20,
                0x00
                });
            cpu.run();

            EXPECT_TRUE(cpu.isAccumulator8Bit() == false);
        }
        TEST_F(CPUOpcodeTest, REP_SetIndex_16BitMode_SetsCorrectPFlags)
        {
            cpu.setFlag(M, true);
            cpu.setFlag(X, true);
            LoadProgram({
                0xC2, 0x10,
                0x00
                });
            cpu.run();

            EXPECT_TRUE(cpu.isIndex8Bit() == false);
        }
        TEST_F(CPUOpcodeTest, REP_SetAccumulatorAndIndex_16BitMode_SetsCorrectPFlags)
        {
            cpu.setFlag(M, true);
            cpu.setFlag(X, true);
            LoadProgram({
                0xC2, 0x30,
                0x00
                });
            cpu.run();

            EXPECT_TRUE(cpu.isAccumulator8Bit() == false);
            EXPECT_TRUE(cpu.isIndex8Bit() == false);
        }
        TEST_F(CPUOpcodeTest, SEP_SetAccumulator_8BitMode_SetsCorrectPFlags)
        {
            cpu.setFlag(M, false);
            cpu.setFlag(X, false);
            LoadProgram({
                0xE2, 0x20,
                0x00
                });
            cpu.run();

            EXPECT_TRUE(cpu.isAccumulator8Bit() == true);
        }
        TEST_F(CPUOpcodeTest, SEP_SetIndex_8BitMode_SetsCorrectPFlags)
        {
            cpu.setFlag(M, false);
            cpu.setFlag(X, false);
            LoadProgram({
                0xE2, 0x10,
                0x00
                });
            cpu.run();

            EXPECT_TRUE(cpu.isIndex8Bit() == true);
        }
        TEST_F(CPUOpcodeTest, SEP_SetAccumulatorIndex_8BitMode_SetsCorrectPFlags)
        {
            cpu.setFlag(M, false);
            cpu.setFlag(X, false);
            LoadProgram({
                0xE2, 0x30,
                0x00
                });
            cpu.run();

            EXPECT_TRUE(cpu.isAccumulator8Bit() == true);
            EXPECT_TRUE(cpu.isIndex8Bit() == true);
        }
        TEST_F(CPUOpcodeTest, REP_Set16BitMode_SetsCorrectPFlags)
        {
            LoadProgram({
                0xE2, 0x30,
                0xC2, 0x10,
                0xC2, 0x20,
                0xE2, 0x10
                });
            cpu.run();

            EXPECT_TRUE(cpu.isAccumulator8Bit() == false);
            EXPECT_TRUE(cpu.isIndex8Bit() == true);
        }
    }
    namespace TestProgram
    {
        TEST_F(CPUOpcodeTest, OpCodeProgramTest_LoadsCorrectValues)
        {
            memory->wram.write(0x0042, 0x54 & 0xFF);
            memory->wram.write(0x0043, 0x76 & 0xFF);
            cpu.registers.D = 0x00;

            LoadProgram({
                // Setup: preload memory at $1234 and $0042 via STA immediate
                0xA9, 0x42,             // LDA #$42
                0x8D, 0x34, 0x12,       // STA $1234 (absolute)

                0xA2, 0x05,             // LDX #$05
                0x8E, 0x78, 0x56,       // STX $5678 (absolute)

                0xA0, 0x99,             // LDY #$99
                0x8C, 0x10, 0x10,       // STY $1010 (absolute)

                // Direct page: write $21 to DP location $10, then read it
                0xA9, 0x21,             // LDA #$21
                0x85, 0x10,             // STA $10 (direct page)
                0xA9, 0x00,             // LDA #$00 (clear A)
                0xA5, 0x10,             // LDA $10
                0x8D, 0xB1, 0x2C,       // STA $2CB1 for test assert

                // Direct page, X
                0xA9, 0x55,             // LDA #$55
                0x95, 0x11,             // STA $11,X   (X = 5, writes to $16)
                0xA9, 0x00,             // LDA #$00
                0xB5, 0x11,             // LDA $11,X   (loads from $16)
                0x8D, 0x43, 0xDC,       // STA $DC43 for test assert

                // Absolute, X
                0xA9, 0x66,             // LDA #$66
                0x9D, 0x00, 0x20,       // STA $2000,X ($2005)
                0xA9, 0x00,             // LDA #$00
                0xBD, 0x00, 0x20,       // LDA $2000,X ($2005)
                0x8D, 0x7A, 0x12,       // STA $127A for test assert

                // Absolute, Y
                0xA9, 0x77,             // LDA #$77
                0x99, 0x00, 0x30,       // STA $3000,Y ($3099)
                0xA9, 0x00,             // LDA #$00
                0xB9, 0x00, 0x30,       // LDA $3000,Y
                0x8D, 0x7B, 0xB2,       // STA $B27B for test assert

                // Absolute long
                0xA9, 0x88,             // LDA #$88
                0x8F, 0x00, 0x40, 0x7E, // STA $7E4000
                0xA9, 0x00,             // LDA #$00
                0xAF, 0x00, 0x40, 0x7E, // LDA $7E4000
                0x8D, 0x41, 0x44,       // STA $4441 for test assert

                // Direct page indexed indirect, Y
                0xA9, 0xDE,             // LDA #$DE
                0x85, 0x20,             // STA $20
                0xA9, 0x40,             // LDA #$40
                0x85, 0x21,             // STA $21

                0xA9, 0x99,             // LDA #$99
                0x91, 0x20,             // STA ($20),Y

                0xA9, 0x00,             // LDA #$00
                0xB1, 0x20,             // LDA ($20),Y
                0x8D, 0xAA, 0xEE,       // STA $EEAA for test assert

                0x00                    // BRK
                });

            cpu.run();

            EXPECT_EQ(memory->wram.read(0x1234), 0x42); // STA $1234
            EXPECT_EQ(memory->wram.read(0x5678), 0x05); // STX $5678
            EXPECT_EQ(memory->wram.read(0x1010), 0x99); // STY $1010
            EXPECT_EQ(memory->wram.read(0x0010), 0x21); // STA $10 (Direct Page)
            EXPECT_EQ(memory->wram.read(0x2CB1), 0x21); // LDA $10 — expect value in A to now be 0x21
            EXPECT_EQ(memory->wram.read(0x0016), 0x55); // STA $11,X -> $11 + 5 = $16
            EXPECT_EQ(memory->wram.read(0xDC43), 0x55); // LDA $11,X -> $16 again
            EXPECT_EQ(memory->wram.read(0x2005), 0x66); // STA $2000,X -> $2005
            EXPECT_EQ(memory->wram.read(0x127A), 0x66); // LDA $2000,X -> expect 0x66 back
            EXPECT_EQ(memory->wram.read(0x3099), 0x77); // STA $3000,Y -> $3099
            EXPECT_EQ(memory->wram.read(0xB27B), 0x77); // LDA $3000,Y -> expect 0x77
            EXPECT_EQ(memory->wram.read(0x4000), 0x88); // STA $7E4000 (absolute long)
            EXPECT_EQ(memory->wram.read(0x4441), 0x88); // LDA $7E4000
            EXPECT_EQ(memory->wram.read(0x4177), 0x99); // STA ($20),Y — ($20) = $40DE, Y = 0x99 -> $4177
            EXPECT_EQ(memory->wram.read(0xEEAA), 0x99); // LDA ($20),Y — expect A = 0x99
        }
    }
}