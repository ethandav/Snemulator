#include "opcodes.h"
#include "cpu.h"
#include "AddressingModes.h"
#include <iostream>

void register_opcodes(CPU& cpu)
{
#pragma region LDA
	// LDA DP Indexed Indirect, X
	cpu.opcodeTable[0xA1] = [](CPU& cpu)
	{
		uint32_t address = dpIndirectX(cpu);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA Stack Relative
	cpu.opcodeTable[0xA3] = [](CPU& cpu)
	{
		uint32_t addr = stackRelative(cpu);
		cpu.setA(cpu.memory->read(addr));
	};
	// LDA Direct Page
	cpu.opcodeTable[0xA5] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA DP Indirect Long
	cpu.opcodeTable[0xA7] = [](CPU& cpu)
	{
		uint32_t address = dpIndirectLong(cpu);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA Immediate
	cpu.opcodeTable[0xA9] = [](CPU& cpu)
	{
		cpu.setA(immediate(cpu, cpu.isAccumulator8Bit()));
	};
	// LDA Absolute
	cpu.opcodeTable[0xAD] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA AbsoluteLong
	cpu.opcodeTable[0xAF] = [](CPU& cpu)
	{
		uint32_t address = absoluteLong(cpu);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA DP Indirect Indexed, Y
	cpu.opcodeTable[0xB1] = [](CPU& cpu)
	{
		uint32_t address = dpIndirectY(cpu);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA DP Indirect
	cpu.opcodeTable[0xB2] = [](CPU& cpu)
	{
		uint32_t address = dpIndirect(cpu);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA SR Indirect Indexed,Y
	cpu.opcodeTable[0xB3] = [](CPU& cpu)
	{
		uint32_t addr = stackRelativeIndirectY(cpu);
		cpu.setA(cpu.memory->read(addr));
	};
	// LDA Direct Page Indexed, X
	cpu.opcodeTable[0xB5] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu, cpu.registers.X);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA DP Indirect Long Indexed,Y
	cpu.opcodeTable[0xB7] = [](CPU& cpu)
	{
		uint32_t address = dpIndirectLong(cpu, cpu.registers.Y);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA Absolute Indexed,Y
	cpu.opcodeTable[0xB9] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu, cpu.registers.Y);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA Absolute Indexed,X
	cpu.opcodeTable[0xBD] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu, cpu.registers.X);
		cpu.setA(cpu.memory->read(address));
	};
	// LDA AbsoluteLong Indexed,X
	cpu.opcodeTable[0xBF] = [](CPU& cpu)
	{
		uint32_t address = absoluteLong(cpu, cpu.registers.X);
		cpu.setA(cpu.memory->read(address));
	};
#pragma endregion
#pragma region LDX
	// LDX Absolute
	cpu.opcodeTable[0xAE] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu);
		cpu.setIndex(cpu.registers.X, cpu.memory->read(address));
	};
	// LDX Immediate
	cpu.opcodeTable[0xA2] = [](CPU& cpu)
	{
		cpu.setIndex(cpu.registers.X, immediate(cpu, cpu.isIndex8Bit()));
	};
	// LDX Direct Page
	cpu.opcodeTable[0xA6] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu);
		cpu.setIndex(cpu.registers.X,  cpu.memory->read(address));
	};
	// LDX Direct Page Indexed,Y
	cpu.opcodeTable[0xB6] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu, cpu.registers.Y);
		cpu.setIndex(cpu.registers.X,  cpu.memory->read(address));
	};
	// LDX Absolute Indexed,Y
	cpu.opcodeTable[0xBE] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu, cpu.registers.Y);
		cpu.setIndex(cpu.registers.X,  cpu.memory->read(address));
	};
#pragma endregion
#pragma region LDY
	// LDY Immediate
	cpu.opcodeTable[0xA0] = [](CPU& cpu)
	{
			cpu.setIndex(cpu.registers.Y, immediate(cpu, cpu.isIndex8Bit()));
	};
	// LDY Absolute
	cpu.opcodeTable[0xAC] = [](CPU& cpu)
	{
			uint32_t address = absolute(cpu);
			cpu.setIndex(cpu.registers.Y,  cpu.memory->read(address));
	};
	// LDY Direct Page
	cpu.opcodeTable[0xA4] = [](CPU& cpu)
	{
			uint32_t address = directPage(cpu);
			cpu.setIndex(cpu.registers.Y,  cpu.memory->read(address));
	};
	// LDY Direct Page Indexed,X
	cpu.opcodeTable[0xB4] = [](CPU& cpu)
	{
			uint32_t address = directPage(cpu, cpu.registers.X);
			cpu.setIndex(cpu.registers.Y,  cpu.memory->read(address));
	};
	// LDY Absolute Indexed,X
	cpu.opcodeTable[0xBC] = [](CPU& cpu)
	{
			uint32_t address = absolute(cpu, cpu.registers.X);
			cpu.setIndex(cpu.registers.Y,  cpu.memory->read(address));
	};
#pragma endregion
#pragma region STA
	// STA DP Indexed Indirect,X
	cpu.opcodeTable[0x81] = [](CPU& cpu)
	{
		uint32_t address = dpIndirectX(cpu);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA Stack Relative
	cpu.opcodeTable[0x83] = [](CPU& cpu)
	{
		uint32_t address = stackRelative(cpu);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA Direct Page
	cpu.opcodeTable[0x85] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA DP Indirect Long
	cpu.opcodeTable[0x87] = [](CPU& cpu)
	{
		uint32_t address = dpIndirectLong(cpu);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA Absolute
	cpu.opcodeTable[0x8D] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA Absolute Long
	cpu.opcodeTable[0x8F] = [](CPU& cpu)
	{
		uint32_t address = absoluteLong(cpu);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA DP Indirect Indexed,Y
	cpu.opcodeTable[0x91] = [](CPU& cpu)
	{
		uint32_t address = dpIndirectY(cpu);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA DP Indirect
	cpu.opcodeTable[0x92] = [](CPU& cpu)
	{
		uint32_t address = dpIndirect(cpu);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA SR Indirect Indexed,Y
	cpu.opcodeTable[0x93] = [](CPU& cpu)
	{
		uint32_t address = stackRelativeIndirectY(cpu);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA DP Indexed,X
	cpu.opcodeTable[0x95] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu, cpu.registers.X);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA DP Indirect Long Indexed,Y
	cpu.opcodeTable[0x97] = [](CPU& cpu)
	{
		uint32_t address = dpIndirectLong(cpu, cpu.registers.Y);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA Absolute Indexed,Y
	cpu.opcodeTable[0x99] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu, cpu.registers.Y);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA Absolute Indexed,X
	cpu.opcodeTable[0x9D] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu, cpu.registers.X);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
	// STA Absolute Long Indexed,X
	cpu.opcodeTable[0x9F] = [](CPU& cpu)
	{
		uint32_t address = absoluteLong(cpu, cpu.registers.X);
		cpu.memory->write(address, cpu.registers.A & 0xFF);
	};
#pragma endregion
#pragma region STX
	// STX Direct Page
	cpu.opcodeTable[0x86] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu);
		cpu.memory->write(address, cpu.registers.X & 0xFF);
	};
	// STX Absolute
	cpu.opcodeTable[0x8E] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu);
		cpu.memory->write(address, cpu.registers.X & 0xFF);
	};
	// STX DP Indexed,Y
	cpu.opcodeTable[0x96] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu, cpu.registers.Y);
		cpu.memory->write(address, cpu.registers.X & 0xFF);
	};
#pragma endregion
#pragma region STY
	// STY Direct Page
	cpu.opcodeTable[0x84] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu);
		cpu.memory->write(address, cpu.registers.Y & 0xFF);
	};
	// STY Absolute
	cpu.opcodeTable[0x8C] = [](CPU& cpu)
	{
		uint32_t address = absolute(cpu);
		cpu.memory->write(address, cpu.registers.Y & 0xFF);
	};
	// STY DP Indexed,X
	cpu.opcodeTable[0x94] = [](CPU& cpu)
	{
		uint32_t address = directPage(cpu, cpu.registers.X);
		cpu.memory->write(address, cpu.registers.Y & 0xFF);
	};
#pragma endregion
#pragma region REP/SEP
	// REP
	cpu.opcodeTable[0xC2] = [](CPU& cpu)
	{
		uint8_t value = immediate(cpu, true);
		cpu.registers.P &= ~value;
		cpu.updateRegisterSizes();
	};
	// SEP
	cpu.opcodeTable[0xE2] = [](CPU& cpu)
	{
		uint8_t value = immediate(cpu, true);
		cpu.registers.P |= value;
		cpu.updateRegisterSizes();
	};
#pragma endregion
#pragma region Misc
	// BRK
	cpu.opcodeTable[0x00] = [](CPU& cpu)
		{
			cpu.running = false;
		};

	// FATAL
	cpu.opcodeTable[0xFF] = [](CPU& cpu)
		{
			cpu.running = false;
		};
#pragma endregion
}
