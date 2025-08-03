#include "opcodes.h"
#include "cpu.h"
#include "AddressingModes.h"
#include <iostream>

void register_opcodes(CPU& cpu)
{
	// LDA Immediate
	cpu.opcodeTable[0xA9] = [](CPU& cpu)
		{
			cpu.registers.A = immediate(cpu);
		};

	// LDA Absolute
	cpu.opcodeTable[0xAD] = [](CPU& cpu)
		{
			uint32_t address = absolute(cpu);
			cpu.registers.A = cpu.memory->read(address);
		};

	// LDA Absolute Indexed,X
	cpu.opcodeTable[0xBD] = [](CPU& cpu)
		{
			uint32_t address = absolute(cpu, cpu.registers.X);
			cpu.registers.A = cpu.memory->read(address);
		};

	// LDA Absolute Indexed,Y
	cpu.opcodeTable[0xB9] = [](CPU& cpu)
		{
			uint32_t address = absolute(cpu, cpu.registers.Y);
			cpu.registers.A = cpu.memory->read(address);
		};

	// LDA Direct Page
	cpu.opcodeTable[0xA5] = [](CPU& cpu)
		{
			uint32_t address = directPage(cpu);
			cpu.registers.A = cpu.memory->read(address);
		};

	// LDA Direct Page Indexed, X
	cpu.opcodeTable[0xB5] = [](CPU& cpu)
		{
			uint32_t address = directPage(cpu, cpu.registers.X);
			cpu.registers.A = cpu.memory->read(address);
		};

	// LDA DP Indirect
	cpu.opcodeTable[0xB2] = [](CPU& cpu)
		{
			uint32_t address = indirect(cpu);
			cpu.registers.A = cpu.memory->read(address);
		};

	// LDA DP Indirect Indexed, X
	cpu.opcodeTable[0xA1] = [](CPU& cpu)
		{
			uint32_t address = indirect(cpu, cpu.registers.X);
			cpu.registers.A = cpu.memory->read(address);
		};

	// LDA DP Indirect Indexed, Y
	cpu.opcodeTable[0xB1] = [](CPU& cpu)
		{
			uint32_t address = indirect(cpu, cpu.registers.Y);
			cpu.registers.A = cpu.memory->read(address);
		};

	// LDX Absolute
	cpu.opcodeTable[0xAE] = [](CPU& cpu)
		{
			uint32_t address = absolute(cpu);
			cpu.registers.X = cpu.memory->read(address);
		};

	// LDX Immediate
	cpu.opcodeTable[0xA2] = [](CPU& cpu)
		{
			cpu.registers.X = cpu.memory->read(cpu.registers.PC++);
		};

	// STA Absolute
	cpu.opcodeTable[0x8D] = [](CPU& cpu)
		{
			uint32_t address = absolute(cpu);
			cpu.memory->write(address, cpu.registers.A & 0xFF);
		};

	// STA Absolute Indexed,X
	cpu.opcodeTable[0x9D] = [](CPU& cpu)
		{
			uint32_t address = absolute(cpu);
			cpu.memory->write(address, cpu.registers.A & 0xFF);
		};

	// STX Absolute
	cpu.opcodeTable[0x8E] = [](CPU& cpu)
		{
			uint32_t address = absolute(cpu);
			cpu.memory->write(address, cpu.registers.X & 0xFF);
		};

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

}
