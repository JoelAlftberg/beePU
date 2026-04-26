#include "memory/memorycontroller.h"

#include <cstdint>

namespace memory
{

Region MemoryController::getRegion(std::uint16_t address)
{
	if(address <= FIXED_REGION_END)
	{
		return Region::Fixed;
	}
	else if (address <= BANKED_REGION_END)
	{
		return Region::Banked;
	}
	else if (address <= DISPLAY_REGION_END)
	{
		return Region::Display;
	}
	else if (address <= IO_REGION_END)
	{
		return Region::IO;
	}
	else if (address <= ROM_REGION_END)
	{
		return Region::ROM;
	}

	return Region::Invalid;
}

MemoryTarget MemoryController::resolveAddress(std::uint16_t address)
{
	Region region{getRegion(address)};
	MemoryTarget target{nullptr, 0U};

	switch(region)
	{
		case Region::Fixed:
		{
			target.memoryUnit = &fixedRAM_;
			target.offset = address;
			break;
		}
		case Region::Banked:
		{
			target.memoryUnit = &banks_[currentBank_];
			target.offset = address - BANKED_REGION_START;
			break;
		}
		case Region::Display:
		{
			target.memoryUnit = &display_;
			target.offset = address - DISPLAY_REGION_START;
			break;
		}
		case Region::IO:
		{
			target.memoryUnit = &io_;
			target.offset = address - IO_REGION_START;
			break;
		}
		case Region::ROM:
		{
			target.memoryUnit = &rom_;
			target.offset = address - ROM_REGION_START;
		}
		case Region::Invalid:
		{
			return target;
		}
	}
	return target;
}

void MemoryController::write(std::uint8_t value, std::uint16_t address)
{
	MemoryTarget target{resolveAddress(address)};

	if (nullptr != target.memoryUnit or &rom_ != target.memoryUnit)
	{
		target.memoryUnit->write(value, target.offset);
	}
}

void MemoryController::writeWord(std::uint16_t value, std::uint16_t address)
{
	MemoryTarget target{resolveAddress(address)};

	if (nullptr != target.memoryUnit and &rom_ != target.memoryUnit)
	{
		target.memoryUnit->writeWord(value, target.offset);
	}
}

std::uint8_t MemoryController::read(std::uint16_t address)
{
	MemoryTarget target{resolveAddress(address)};

	if (nullptr != target.memoryUnit)
	{
		return target.memoryUnit->read(target.offset);
	}

	return 0U;
}

std::uint16_t MemoryController::readWord(std::uint16_t address)
{
	MemoryTarget target{resolveAddress(address)};

	if (nullptr != target.memoryUnit)
	{
		return target.memoryUnit->readWord(target.offset);
	}

	return 0U;
}

} // namespace memory