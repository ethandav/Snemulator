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
	data = std::vector<uint8_t>(input);
}