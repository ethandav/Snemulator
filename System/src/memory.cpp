#include "memory.h"

void MemoryMap::map(uint32_t start, uint32_t end, MemoryHandler* handler)
{
	MemoryMapEntry entry = { start, end, handler, start };
	regions.push_back(entry);

	uint8_t startBank = start >> 16;
	uint8_t endBank = end >> 16;

	for (uint8_t bank = startBank; bank <= endBank; ++bank)
	{
		if (!bankLUT[bank])
			bankLUT[bank] = std::make_unique<std::vector<MemoryMapEntry>>();

		bankLUT[bank]->push_back(entry);
	}

	if (regions.size() > 8)
		useLUT = true;
}

uint8_t MemoryMap::read(uint32_t address)
{
	if (useLUT)
	{
		uint8_t bank = address >> 16;
		for (auto& entry : *bankLUT[bank])
		{
			if (address >= entry.start && address <= entry.end)
			{
				lastReadData = entry.handler->read(address - entry.base);
				return lastReadData;
			}
		}
	}
	else
	{
		for (auto& entry : regions)
		{
			if (address >= entry.start && address <= entry.end)
			{
				lastReadData = entry.handler->read(address - entry.base);
				return lastReadData;
			}
		}
	}
	return lastReadData;
}

void MemoryMap::write(uint32_t address, uint8_t value)
{
	if (useLUT)
	{
		uint8_t bank = address >> 16;
		for (auto& entry : *bankLUT[bank])
		{
			if (address >= entry.start && address <= entry.end)
			{
				entry.handler->write(address - entry.base, value);
				return;
			}
		}
	}
	else
	{
		for (auto& entry : regions)
		{
			if (address >= entry.start && address <= entry.end)
			{
				entry.handler->write(address - entry.base, value);
				return;
			}
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