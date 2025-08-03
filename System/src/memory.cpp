#include "memory.h"

void MemoryMap::map(uint32_t start, uint32_t end, MemoryHandler* handler)
{
	regions.push_back({ start, end, handler });
}

uint8_t MemoryMap::read(uint32_t address)
{
	for (auto& region : regions)
	{
		if (address >= region.start && address <= region.end)
		{
			return region.handler->read(address - region.start);
		}
	}
	return 0xFF;
}

void MemoryMap::write(uint32_t address, uint8_t value)
{
	for (auto& region : regions)
	{
		if (address >= region.start && address <= region.end)
		{
			region.handler->write(address - region.start, value);
			return;
		}
	}
}

uint8_t RAM::read(uint32_t address)
{
	return data[address];
}

void RAM::write(uint32_t address, uint8_t value)
{
	data[address] = value;
}

void RAM::load(size_t size)
{
	data = std::vector<uint8_t>(size, 0xFF);
}

uint8_t ROM::read(uint32_t address)
{
	if (address < data.size())
	{
		return data[address];
	}
	return 0xFF;
}

void ROM::load(const std::vector<uint8_t> input)
{
	data = input;
}

void Memory::init()
{
	wram.load(0x20000);
	map.map(0x7E0000, 0x7FFFFF, &wram);
}

void Memory::loadProgram(const std::vector<uint8_t>& program)
{
	rom.load(program);
	map.map(0x008000, 0x008000 + rom.size() - 1, &rom);
	mirrorWRam();
}

void Memory::mirrorWRam()
{
	for (uint8_t bank = 0x00; bank <= 0x3F; ++bank)
		map.map((bank << 16), (bank << 16) + 0x1FFF, &wram);
	for (uint8_t bank = 0x80; bank <= 0xBF; ++bank)
		map.map((bank << 16), (bank << 16) + 0x1FFF, &wram);
}