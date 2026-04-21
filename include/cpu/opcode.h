#pragma once

#include <cstdint>

namespace cpu
{

enum class Opcode : uint8_t
{
	ADD,
	AND,
	HLT,
	JMP,
	LDA,
	MOV,
	NOP,
	NOT,
	OR,
	STA,
	SUB,
};

} // namespace cpu