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
}