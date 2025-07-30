#include "cpu.h"
#include "memory.h"
#include <iostream>

int main()
{
	CPU cpu = {};
	RAM ram(0x20000);
	ROM rom({0xA9, 0x05, 0xA2, 0x03, 0x00});
	
	MemoryMap memory = {};
	memory.map(0x7E0000, 0x7FFFFF, &ram);
	memory.map(0x008000, 0x008004, &rom);

	cpu.memory = &memory;
	cpu.registers.PC = 0x008000;
	cpu.running = true;

	cpu.run();

	std::cout << "A = " << (int)cpu.registers.A << ", X = " << (int)cpu.registers.X << std::endl;

	return 0;
}