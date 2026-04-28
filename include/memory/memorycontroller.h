#pragma once

#include <array>
#include <cstdint>
#include <iostream>

#include "arch.h"
#include "memory/memory.h"

namespace memory
{

enum class Region : std::uint8_t
{
	Fixed,
	Banked,
	Display,
	IO,
	ROM,
	Invalid
};

struct MemoryTarget
{
	Memory* memoryUnit;
	std::uint16_t offset;
};

class MemoryController
{

public:

	void write(std::uint8_t value, std::uint16_t address);
	void writeWord(std::uint16_t value, std::uint16_t address);
	std::uint8_t read(std::uint16_t address);
	std::uint16_t readWord(std::uint16_t address);

	std::uint16_t readStack(std::uint16_t address);
	void writeStack(std::uint16_t value, std::uint16_t address);

	void reset()
	{
		fixedRAM_.clear();
		for (memory::Memory& bank : banks_)
		{
			bank.clear();
		}
		display_.clear();
		io_.clear();
		rom_.clear();
	}

	inline void switchBank(std::uint8_t bankIndex)
	{
		if(bankIndex >= NUM_BANKS or STACK_BANK_INDEX == bankIndex)
		{
			return;
		}
		currentBank_ = bankIndex;
	}

private:
	inline Region getRegion(std::uint16_t address);
	MemoryTarget resolveAddress(std::uint16_t address);

	memory::Memory fixedRAM_{FIXED_REGION_SIZE};
	std::array<Memory, NUM_BANKS> banks_;
	memory::Memory display_{DISPLAY_MEM_SIZE};
	memory::Memory io_{IO_MEM_SIZE};
	memory::Memory rom_{ROM_SIZE};

	std::uint8_t currentBank_{0U};
	
};

} // namespace memory