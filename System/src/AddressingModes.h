#pragma once
#include "cpu.h"
#include <cstdint>

inline uint16_t immediate(CPU& cpu, bool in8BitMode)
{
    if (in8BitMode)
        return cpu.memory->read(cpu.registers.PC++) & 0xFF;
    else
    {
        uint32_t lo = cpu.memory->read(cpu.registers.PC++);
        uint32_t hi = cpu.memory->read(cpu.registers.PC++);
        return (hi << 8) | lo;
    }
}

inline uint32_t absolute(CPU& cpu, uint16_t index = 0x00)
{
    uint32_t lo = cpu.memory->read(cpu.registers.PC++);
    uint32_t hi = cpu.memory->read(cpu.registers.PC++);
    uint32_t address = (hi << 8) | lo;
    return (cpu.registers.DBR << 16) | ((address + index) & 0xFFFF);
}

inline uint32_t directPage(CPU& cpu, uint16_t index = 0x00) {
    uint8_t offset = cpu.memory->read(cpu.registers.PC++);
    uint16_t dpOffset = (offset + index) & 0xFF;
    return (0x00 << 16) | ((cpu.registers.D + dpOffset) & 0xFFFF);
}

inline uint32_t dpIndirectX(CPU& cpu)
{
    uint8_t base = cpu.memory->read(cpu.registers.PC++);
    uint8_t offset = (base + cpu.registers.X) & 0xFF;
    uint16_t address = (cpu.registers.D + offset) & 0xFFFF;
    uint8_t lo = cpu.memory->read((0x00 << 16) | address);
    uint8_t hi = cpu.memory->read((0x00 << 16) | ((address + 1) & 0xFFFF));
    return (cpu.registers.DBR << 16) | ((hi << 8) | lo);
}

inline uint32_t dpIndirectY(CPU& cpu)
{
    uint8_t operand = cpu.memory->read(cpu.registers.PC++);
    uint16_t base = (cpu.registers.D + operand) & 0xFFFF;
    uint8_t lo = cpu.memory->read(0x000000 | (base & 0xFF));
    uint8_t hi = cpu.memory->read(0x000000 | ((base + 1) & 0xFF));
    uint16_t pointer = (hi << 8) | lo;
    return (cpu.registers.DBR << 16) | ((pointer + cpu.registers.Y) & 0xFFFF);
}

inline uint32_t dpIndirect(CPU& cpu)
{
    uint8_t operand = cpu.memory->read(cpu.registers.PC++);
    uint16_t base = (cpu.registers.D + operand) & 0xFFFF;
    uint8_t lo = cpu.memory->read(0x000000 | (base & 0xFF));
    uint8_t hi = cpu.memory->read(0x000000 | ((base + 1) & 0xFF));
    uint16_t pointer = (hi << 8) | lo;
    return (cpu.registers.DBR << 16) | (pointer & 0xFFFF);
}

inline uint32_t absoluteLong(CPU& cpu, uint16_t index = 0x00)
{
    uint8_t lo = cpu.memory->read(cpu.registers.PC++);
    uint8_t mid = cpu.memory->read(cpu.registers.PC++);
    uint8_t hi = cpu.memory->read(cpu.registers.PC++);
    return ((hi << 16) | (mid << 8) | lo) + index;
}

inline uint32_t dpIndirectLong(CPU& cpu, uint16_t index = 0x00)
{
    uint8_t dpOffset = cpu.memory->read(cpu.registers.PC++);
    uint32_t address = (cpu.registers.D + dpOffset) & 0xFF;
    uint8_t lo = cpu.memory->read(address);
    uint8_t mid = cpu.memory->read(address + 1);
    uint8_t high = cpu.memory->read(address + 2);
    uint32_t pointer = (high << 16) | (mid << 8) | lo;
    return pointer + index;
}

inline uint32_t stackRelative(CPU& cpu)
{
    uint8_t offset = cpu.memory->read(cpu.registers.PC++);
    uint16_t address = (cpu.registers.S + offset) & 0xFF;
    return 0x000100 | address;
}

inline uint32_t stackRelativeIndirectY(CPU& cpu)
{
    uint8_t offset = cpu.memory->read(cpu.registers.PC++);
    uint16_t spAddress = (cpu.registers.S + offset) & 0xFF;
    uint16_t base = 0x0100 | spAddress;
    uint8_t lo = cpu.memory->read(base);
    uint8_t hi = cpu.memory->read(0x0100 | ((spAddress + 1) & 0xFF));
    uint16_t pointer = (hi << 8) | lo;
    return (cpu.registers.DBR << 16) | ((pointer + cpu.registers.Y) & 0xFFFF);
}