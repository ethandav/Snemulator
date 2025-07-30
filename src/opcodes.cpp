#include "opcodes.h"
#include "cpu.h"

void register_opcodes(CPU& cpu)
{
	cpu.opcodeTable[0xA9] = [](CPU& cpu)
		{
			cpu.registers.A = cpu.memory->read(cpu.registers.PC++);
		};

	cpu.opcodeTable[0xA2] = [](CPU& cpu)
		{
			cpu.registers.X = cpu.memory->read(cpu.registers.PC++);
		};

	cpu.opcodeTable[0x00] = [](CPU& cpu)
		{
			cpu.running = false;
		};
}
