#pragma once
#include "cpu.h"
#include "addressing_utils.h"
#include <cstdint>

inline uint16_t dpRead16(const CPU& cpu, uint8_t dpOff)
{
    uint16_t base = cpu.registers.D;
    uint16_t a0 = uint16_t(base + uint8_t(dpOff));
    uint16_t a1 = uint16_t(base + uint8_t(dpOff + 1));
    uint8_t lo = cpu.memory->read(addr::bank0(a0));
    uint8_t hi = cpu.memory->read(addr::bank0(a1));
    return uint16_t((hi << 8) | lo);
}
inline uint32_t dpRead24(const CPU& cpu, uint8_t dpOff)
{
    uint16_t base = cpu.registers.D;
    uint16_t a0 = uint16_t(base + uint8_t(dpOff));
    uint16_t a1 = uint16_t(base + uint8_t(dpOff + 1));
    uint16_t a2 = uint16_t(base + uint8_t(dpOff + 2));
    uint8_t lo = cpu.memory->read(addr::bank0(a0));
    uint8_t mi = cpu.memory->read(addr::bank0(a1));
    uint8_t hi = cpu.memory->read(addr::bank0(a2));
    return (uint32_t(hi) << 16) | (uint32_t(mi) << 8) | lo;
}

inline uint16_t immediate(CPU& cpu, bool in8BitMode)
{
    if (in8BitMode)
        return cpu.fetch8();
    else
        return cpu.fetch16();
}


inline uint32_t absolute(CPU& cpu, uint16_t index = 0)
{
    uint16_t addr = cpu.fetch16();
    return addr::withDBR(cpu.registers.DBR, uint16_t(addr + index));
}

inline uint32_t absoluteX(CPU& cpu) { return absolute(cpu, cpu.registers.X); }
inline uint32_t absoluteY(CPU& cpu) { return absolute(cpu, cpu.registers.Y); }

inline uint32_t directPage(CPU& cpu, uint16_t index = 0)
{
    uint8_t off = cpu.fetch8();
    uint16_t dpOff = uint8_t(off + uint8_t(index));
    return addr::bank0(uint16_t(cpu.registers.D + dpOff));
}

inline uint32_t dpIndirectX(CPU& cpu)
{
    uint8_t op = cpu.fetch8();
    uint8_t off = uint8_t(op + cpu.registers.X);
    uint16_t ptr = dpRead16(cpu, off);
    return addr::withDBR(cpu.registers.DBR, ptr);
}

inline uint32_t dpIndirectY(CPU& cpu)
{
    uint8_t op = cpu.fetch8();
    uint16_t ptr = dpRead16(cpu, op);
    return addr::withDBR(cpu.registers.DBR, uint16_t(ptr + cpu.registers.Y));
}

inline uint32_t dpIndirect(CPU& cpu)
{
    uint8_t op = cpu.fetch8();
    uint16_t ptr = dpRead16(cpu, op);
    return addr::withDBR(cpu.registers.DBR, ptr);
}

inline uint32_t absoluteLong(CPU& cpu, uint16_t index = 0)
{
    uint32_t base = cpu.fetch24();
    return (base + uint32_t(index)) & 0xFFFFFFu;
}

inline uint32_t dpIndirectLong(CPU& cpu, uint16_t index = 0)
{
    uint8_t op = cpu.fetch8();
    uint32_t base = dpRead24(cpu, op);
    return (base + uint32_t(index)) & 0xFFFFFFu;
}

inline uint32_t dpIndirectLongY(CPU& cpu)
{
    uint8_t op = cpu.fetch8();
    uint32_t base = dpRead24(cpu, op);
    return (base + uint32_t(cpu.registers.Y)) & 0xFFFFFFu;
}

inline uint32_t stackRelative(CPU& cpu)
{
    uint8_t d = cpu.fetch8();

    if (cpu.registers.E) {
        uint8_t base8 = uint8_t((uint8_t(cpu.registers.S) + d) & 0xFF);
        return addr::bank0(uint16_t(0x0100 | base8));
    }
    else {
        uint16_t page = cpu.registers.S & 0xFF00;
        uint8_t  base8 = uint8_t(((cpu.registers.S & 0x00FF) + d) & 0xFF);
        return addr::bank0(uint16_t(page | base8));
    }
}

inline uint32_t stackRelativeIndirectY(CPU& cpu)
{
    uint8_t d = cpu.fetch8();

    uint16_t page = cpu.registers.E ? 0x0100 : (cpu.registers.S & 0xFF00);
    uint8_t  base8 = uint8_t(((cpu.registers.S & 0x00FF) + d) & 0xFF);

    uint8_t lo = cpu.memory->read(addr::bank0(uint16_t(page | base8)));
    uint8_t hi = cpu.memory->read(addr::bank0(uint16_t(page | uint8_t(base8 + 1))));

    uint16_t ptr = uint16_t((hi << 8) | lo);
    return addr::withDBR(cpu.registers.DBR, uint16_t(ptr + cpu.registers.Y));
}

inline uint32_t absoluteIndirect(CPU& cpu)
{
    uint16_t addr = cpu.fetch16();
    uint32_t p = addr::withPBR(cpu.registers.PBR, addr);
    uint8_t lo = cpu.memory->read(p);
    uint8_t hi = cpu.memory->read(addr::withPBR(cpu.registers.PBR, uint16_t(addr + 1)));
    uint16_t dst = uint16_t((hi << 8) | lo);
    return addr::withPBR(cpu.registers.PBR, dst);
}

inline uint32_t absoluteIndexedIndirect(CPU& cpu)
{
    uint16_t base = cpu.fetch16();
    uint16_t addr = uint16_t(base + cpu.registers.X);
    uint8_t lo = cpu.memory->read(addr::withPBR(cpu.registers.PBR, addr));
    uint8_t hi = cpu.memory->read(addr::withPBR(cpu.registers.PBR, uint16_t(addr + 1)));
    uint16_t dst = uint16_t((hi << 8) | lo);
    return addr::withPBR(cpu.registers.PBR, dst);
}

inline uint32_t absoluteIndirectLong(CPU& cpu)
{
    uint16_t addr = cpu.fetch16();
    uint8_t lo = cpu.memory->read(addr::bank0(addr));
    uint8_t mi = cpu.memory->read(addr::bank0(uint16_t(addr + 1)));
    uint8_t hi = cpu.memory->read(addr::bank0(uint16_t(addr + 2)));
    return (uint32_t(hi) << 16) | (uint32_t(mi) << 8) | lo;
}
