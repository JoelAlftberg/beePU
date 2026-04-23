#pragma once

#include <cstdint>

#include "memory/register.h"

namespace cpu
{

class ProgramCounter : public memory::Register
{

public:

	void increment()
	{
		value_ += INSTRUCTION_WIDTH;
	}

	void jump(std::uint16_t address)
	{
		write(address);
	}

	void branch(std::uint16_t offset)
	{
		value_ += offset;
	}

	ProgramCounter() = default;
	~ProgramCounter() = default;

};

} // namespace cpu