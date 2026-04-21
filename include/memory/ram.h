#pragma once

#include "arch.h"

#include <cstdint>

namespace memory
{

class RAM
{

public:
	
	void write(std::uint8_t value, std::uint16_t address)
	{
		data_[address] = value;
	}

	void writeWord(std::uint16_t value, std::uint16_t address)
	{
		data_[address] 		= value & 0x00FF;
		data_[address + 1] 	=  (value >> BYTE_WIDTH);
	}

	std::uint8_t read(std::uint16_t address) const
	{
		return data_[address];
	}

	std::uint16_t readWord(std::uint16_t address) const
	{
		std::uint16_t word{0U};
		word |= data_[address];
		word |= (data_[address + 1] << BYTE_WIDTH);

		return word;
	}

private:
	std::uint8_t data_[RAM_SIZE];

};

} // namespace memory