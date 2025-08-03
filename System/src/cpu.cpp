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
	std::cout << "P: " << (int)registers.P << "\n";
	std::cout << "Check M: " << ((registers.P & PFlags::M) != 0) << "\n";
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
}

void CPU::setFlag(PFlags flag, bool enabled)
{
	if (enabled)registers.P |= flag;
	else registers.P &= ~flag;
}

bool CPU::isIn8BitMode()
{
	if ((registers.P & M) != 0)
		return true;
	else
		return false;
}