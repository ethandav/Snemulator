#pragma once
#include "cpu.h"
#include <cstdint>

inline uint32_t absolute(CPU& cpu) {
    uint16_t lo = cpu.memory->read(cpu.registers.PC++);
    uint16_t hi = cpu.memory->read(cpu.registers.PC++);
    uint16_t addr = (hi << 8) | lo;

    return (cpu.registers.DBR << 16) | addr;
}