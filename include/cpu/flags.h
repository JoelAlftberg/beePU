#pragma once

#include <cstdint>

namespace cpu
{

enum class Flag : std::uint8_t {
	Zero = 0U,
	Negative = 1U,
	Carry = 2U,
	Overflow = 3U,
};

class Flags
{

public:

	void setFlag(Flag f)
	{
		flags_ |= (1U << static_cast<std::uint8_t>(f));
	}

	void clearFlag(Flag f)
	{
		flags_ &= ~(1U << static_cast<std::uint8_t>(f));
	}

	bool checkFlag(Flag f) const
	{
		return (flags_ >> static_cast<std::uint8_t>(f)) & 1U; 
	}

	Flags() = default;
	~Flags() = default;

private:
	std::uint16_t flags_{0U};

};

} // namespace cpu


