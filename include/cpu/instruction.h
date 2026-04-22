#pragma once

#include <cstdint>

#include "memory/register.h"

namespace cpu
{

struct Instruction
{
	std::uint8_t id{0U};
	memory::Registers reg1{0U};
	memory::Registers reg2{0U};
	std::uint16_t immediate{0U};
	std::uint16_t raw{0U};
	
};

} // namespace cpu
