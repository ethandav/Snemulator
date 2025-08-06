#pragma once
#include "memory.h"
#include <cstdint>
#include <array>

struct Registers
{
	uint8_t		P	= 0;	//Processor Status, holds various important flags
	uint8_t		DBR = 0;	//Data bank register, holds the default bank for memory transfers
	uint8_t		PBR = 0;	//Program Bank, holds the bank address of all instruction fetches
	uint16_t	PC	= 0;	//Program counter 
	uint16_t	A	= 0;	//The accumulator. This is the math register. It stores one of two operands or the result of most arithmetic and logical operations
	uint16_t	X	= 0;	//The index registers. These can be used to reference memory, to pass data to memory, or as counters for loops
	uint16_t	Y	= 0;	
	uint16_t	S	= 0;	//The stack pointer, points to the next available(unused) location on the stack
	uint16_t	D	= 0;	//Direct page register, used for direct page addressing modes
};

enum PFlags
{
	C = 0x01,  // Carry
	Z = 0x02,  // Zero
	I = 0x04,  // IRQ Disable
	D = 0x08,  // Decimal Mode
	X = 0x10,  // Index Register size (1 = 8-bit)
	M = 0x20,  // Accumulator size (1 = 8-bit)
	V = 0x40,  // Overflow
	N = 0x80   // Negative
};

class CPU
{
public:
	CPU();
	void run();
	void step();
	void setFlag(PFlags flag, bool enabled);
	bool isAccumulator8Bit();
	bool isIndex8Bit();

	Registers registers = {};
	MemoryMap* memory;
	bool running = false;
private:
	using OpcodeHandler = void(*)(CPU&);
	std::array<OpcodeHandler, 256> opcodeTable;

	void setDefaultFlags();
	void setA(uint16_t value);
	void setIndex(uint16_t& index, uint16_t value);
	void updateRegisterSizes();

	friend void register_opcodes(CPU&);
};