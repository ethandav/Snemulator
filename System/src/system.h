#pragma once
#include "cpu.h"

class System
{
public:
	System();
	void init();
	void start();
	void loadRom(const std::vector<uint8_t>& input);
private:
	CPU cpu = {};
	MemoryMap memory = {};
	RAM ram = {};
	ROM rom = {};
};