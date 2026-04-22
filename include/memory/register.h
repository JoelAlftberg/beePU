#pragma once

#include <cstdint>
#include "arch.h"

namespace memory
{

enum class Registers : uint8_t {
	R0 	= 0,
	R1 	= 1,
	R2 	= 2,
	R3 	= 3,
	R4 	= 4,
	R5 	= 5,
	R6 	= 6,
	R7 	= 7,
	R8 	= 8,
	R9 	= 9,
	R10 = 10,
	R11 = 11,
	R12 = 12,
	R13 = 13,
	R14 = 14,
	R15 = 15,
};

class Register
{

public:

	std::uint16_t read() const
	{
		return value_;
	}

	void write(std::uint16_t word)
	{
		value_ = word;
	}

	Register() = default;
	~Register() = default;
	
protected:
	
	std::uint16_t value_{0U};

};

} // namespace memory