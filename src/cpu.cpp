#include "cpu.h"
#include "opcodes.h"

CPU::CPU()
{
	memory = {};
	opcodeTable = {};

	register_opcodes(*this);
}

void CPU::run()
{
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