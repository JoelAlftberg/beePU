#pragma once

#include <cstdint>
#include "arch.h"

namespace memory
{

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