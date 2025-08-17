#include "cpu.h"
#include "opcodes.h"
#include "addressing_utils.h"
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

uint8_t CPU::fetch8()
{
	uint8_t v = memory->read(addr::withPBR(registers.PBR, registers.PC));
	registers.PC = uint16_t(registers.PC + 1);
	return v;
}
uint16_t CPU::fetch16() {
	uint8_t lo = fetch8();
	uint8_t hi = fetch8();
	return uint16_t((hi << 8) | lo);
}
uint32_t CPU::fetch24() {
	uint8_t lo = fetch8();
	uint8_t mi = fetch8();
	uint8_t hi = fetch8();
	return (uint32_t(hi) << 16) | (uint32_t(mi) << 8) | lo;
}