#include <iostream>
#include "system.h"

System::System()
{

}

void System::init()
{
	ram.load(0x20000);

	memory.map(0x7E0000, 0x7FFFFF, &ram);
	memory.map(0x008000, 0x008000 + rom.size() - 1, &rom);

	cpu.memory = &memory;
	cpu.registers.PC = 0x008000;
	cpu.registers.DBR = 0x7E;
}

void System::start()
{
	cpu.running = true;
	cpu.run();
}

void System::loadRom(const std::vector<uint8_t>& input)
{
	rom.load(input);
}