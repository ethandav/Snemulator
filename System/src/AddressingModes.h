#pragma once
#include "cpu.h"
#include <cstdint>

inline uint16_t immediate(CPU& cpu)
{
    if (cpu.isIn8BitMode())
        return cpu.memory->read(cpu.registers.PC++);
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
    return (cpu.registers.DBR << 16) | address + index;
}

inline uint32_t directPage(CPU& cpu, uint16_t index = 0x00) {
    uint8_t offset = cpu.memory->read(cpu.registers.PC++);
    return (0x00 << 16) | ((cpu.registers.D + offset + index) & 0xFFFF);
}

inline uint32_t indirect(CPU& cpu, uint16_t index = 0x00)
{
    uint8_t base = cpu.memory->read(cpu.registers.PC++);
    uint8_t offset = (base + index) & 0xFF;
    uint8_t lo = cpu.memory->read(cpu.registers.D + offset);
    uint8_t hi = cpu.memory->read(cpu.registers.D + ((offset + 1) & 0xFF));
    return (cpu.registers.DBR << 16) | ((hi << 8) | lo);
}