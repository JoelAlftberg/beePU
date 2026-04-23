#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "cpu/flags.h"
#include "cpu/instruction.h"
#include "cpu/programcounter.h"
#include "memory/ram.h"
#include "memory/register.h"
#include "arch.h"

namespace cpu
{

class CPU
{

public:

	void printState();
	void loadProgram(const std::vector<uint16_t>& program, uint16_t start_addr = 0);

	bool halted() const { return halted_; }

	std::uint16_t fetch();
	std::uint16_t getProgramCounter();

	Instruction decode(std::uint16_t instruction_bits);

	void execute(Instruction instruction);
	void step();

	CPU();
	~CPU() = default;

private:

	memory::Register registers_[REG_AMOUNT];
	memory::RAM ram_;
	cpu::ProgramCounter pc_;
	cpu::Flags flags_;
	bool halted_ = false;

	using Handler = void (CPU::*)(const Instruction&);
	Handler opcode_table_[64U]{};

	void executeADD(const Instruction& instruction);
	void executeAND(const Instruction& instruction);
	void executeBEQ(const Instruction& instruction);
	void executeBGT(const Instruction& instruction);
	void executeBLT(const Instruction& instruction);
	void executeBNE(const Instruction& instruction);
	void executeHLT(const Instruction& instruction);
	void executeJMP(const Instruction& instruction);
	void executeJMPI(const Instruction& instruction);
	void executeLDA(const Instruction& instruction);
	void executeLLI(const Instruction& instruction);
	void executeLUI(const Instruction& instruction);
	void executeMOV(const Instruction& instruction);
	void executeNOP(const Instruction& instruction);
	void executeNOT(const Instruction& instruction);
	void executeOR(const Instruction& instruction);
	void executeSTA(const Instruction& instruction);
	void executeSUB(const Instruction& instruction);
	void executeXOR(const Instruction& instruction);

};

} // namespace cpu