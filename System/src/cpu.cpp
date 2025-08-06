#include "cpu.h"
#include "opcodes.h"
#include <iostream>

CPU::CPU()
{
	memory = {};
	opcodeTable = {};

	register_opcodes(*this);
}

void CPU::run()
{
	setDefaultFlags();
	while (running)
	{
		step();
	}
}

void CPU::step()
{
	uint8_t opcode = memory->read(registers.PC++);

	auto& handler = opcodeTable[opcode];
	if (handler)
	{
		handler(*this);
	}
}

void CPU::setDefaultFlags()
{
	setFlag(M, true);
	setFlag(X, true);
}

void CPU::setFlag(PFlags flag, bool enabled)
{
	if (enabled)registers.P |= flag;
	else registers.P &= ~flag;
}

bool CPU::isAccumulator8Bit()
{
	return registers.P & 0x20;
}

bool CPU::isIndex8Bit()
{
	return registers.P & 0x10;
}

void CPU::setA(uint16_t value)
{
	registers.A = isAccumulator8Bit() ? value & 0x00FF : value;
}

void CPU::setIndex(uint16_t& index, uint16_t value)
{
	index = isIndex8Bit() ? value & 0x00FF : value;
}

void CPU::updateRegisterSizes()
{
	if (isAccumulator8Bit())
		registers.A &= 0x00FF;

	if (isIndex8Bit()) {
		registers.X &= 0x00FF;
		registers.Y &= 0x00FF;
	}
}