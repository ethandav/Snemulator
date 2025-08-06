#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <memory>

struct MemoryHandler
{
	virtual uint8_t read(uint32_t address) = 0;
	virtual void write(uint32_t address, uint8_t value) = 0;
	virtual ~MemoryHandler() = default;
};

struct MemoryMapEntry
{
	uint32_t start = 0;
	uint32_t end = 0;
	MemoryHandler* handler = nullptr;
	uint32_t base = 0;
};

class MemoryMap
{
public:
	void map(uint32_t start, uint32_t end, MemoryHandler* handler);
	uint8_t read(uint32_t address);
	void write(uint32_t address, uint8_t value);

	bool useLUT = false;
private:
	std::vector<MemoryMapEntry> regions = {};
	static constexpr size_t BANK_COUNT = 256;
	std::array<std::vector<MemoryMapEntry>, BANK_COUNT> bankLUT = {};
	uint8_t lastReadData = 0x00;
};

class RAM : public MemoryHandler
{
public:
	RAM() {};
	RAM(size_t size) : data(size, 0x00) {};
	uint8_t read(uint32_t address) override;
	void write(uint32_t address, uint8_t value) override;
	void load(size_t size);
private:
	std::vector<uint8_t> data;
};

class ROM : public MemoryHandler
{
public:
	ROM() {};
	ROM(const std::vector<uint8_t>& input) : data(input) {};
	uint8_t read(uint32_t address) override;
	void write(uint32_t address, uint8_t value) override {}
	size_t size() const { return data.size(); }
	void load(const std::vector<uint8_t> input);
private:
	std::vector<uint8_t> data;
};

class Memory
{
public:
	Memory() {};
	void init();
	void loadProgram(const std::vector<uint8_t>& program);

	MemoryMap map = {};
	RAM wram = {};
	ROM rom = {};
private:
	void mirrorWRam();
};