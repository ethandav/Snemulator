#include "cpu.h"
#include "memory.h"
#include <iostream>

int main()
{
	CPU cpu = {};
	RAM ram(0x20000);
	ROM rom({
		0xA9, 0x42,
		0xA2, 0x05,
		0x8D, 0x34, 0x12,
		0x00
	});
	
	MemoryMap memory = {};
	memory.map(0x7E0000, 0x7FFFFF, &ram);
	memory.map(0x008000, 0x008000 + rom.size() - 1, &rom);

	cpu.memory = &memory;
	cpu.registers.PC = 0x008000;
	cpu.registers.DBR = 0x7E;
	cpu.running = true;

	cpu.run();

	uint8_t result = ram.read(0x1234);
	std::cout << "A = " << cpu.registers.A << ", X = " << cpu.registers.X << std::endl;
	std::cout << "RAM[0x1234] = " << (int)result << std::endl;

	return 0;
}