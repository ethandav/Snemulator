#pragma once
#include <cstdint>

namespace addr
{
	inline uint32_t bank0(uint16_t a) { return (0u << 16) | a; }
	inline uint32_t withDBR(uint8_t dbr, uint16_t a) { return (uint32_t(dbr) << 16) | a; }
	inline uint32_t withPBR(uint8_t  pbr, uint16_t a) { return (uint32_t(pbr) << 16) | a; }
}