#pragma once

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "cpu/flags.h"
#include "cpu/instruction.h"
#include "cpu/programcounter.h"
#include "memory/memory.h"
#include "memory/memorycontroller.h"
#include "memory/register.h"
#include "arch.h"

namespace cpu
{

struct Status
{
	std::uint16_t pc;
	std::array<std::uint16_t, REG_AMOUNT> registers;
	std::uint16_t flags;
	bool halted;
};

class CPU
{

public:

	void loadProgram(const std::vector<uint16_t>& program, uint16_t start_addr = 0);
	
	Status getStatus();
	std::uint16_t getProgramCounter();
	std::uint16_t getRegValue(std::string regIndex);
	void clearRegisters();

	std::uint16_t readMemory(std::uint16_t address);
	std::vector<uint16_t> readMemoryRange(std::uint16_t startAddr, std::uint16_t endAddr);

	void incStackPointer()
	{
		std::uint16_t stackPointer = registers_[STACK_POINTER_REGISTER].read();
		if (stackPointer <= BANK_MEM_SIZE - 1)
		{
			registers_[STACK_POINTER_REGISTER].write(stackPointer + 2);
		}
	}

	void decStackPointer()
	{
		std::uint16_t stackPointer = registers_[STACK_POINTER_REGISTER].read();
		if (stackPointer != 0)
		{
			registers_[STACK_POINTER_REGISTER].write(stackPointer - 2);
		}
	}

	bool halted() const { return halted_; }

	std::uint16_t fetch();
	Instruction decode(std::uint16_t instruction_bits);
	void execute(Instruction instruction);
	void step();
	void reset();

	CPU();
	~CPU() = default;

private:

	std::array<memory::Register, REG_AMOUNT> registers_;
	memory::MemoryController memController_{};
	cpu::ProgramCounter pc_;
	cpu::Flags flags_;
	bool halted_ = false;

	using Handler = void (CPU::*)(const Instruction&);
	std::array<Handler, 64U> opcode_table_{};

	void executeADD(const Instruction& instruction);
	void executeAND(const Instruction& instruction);
	void executeBEQ(const Instruction& instruction);
	void executeBGT(const Instruction& instruction);
	void executeBLT(const Instruction& instruction);
	void executeBNE(const Instruction& instruction);
	void executeBANK(const Instruction& instruction);
	void executeCALL(const Instruction& instruction);
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
	void executePOP(const Instruction& instruction);
	void executePUSH(const Instruction& instruction);
	void executeRET(const Instruction& instruction);
	void executeSTA(const Instruction& instruction);
	void executeSUB(const Instruction& instruction);
	void executeXOR(const Instruction& instruction);

};

} // namespace cpu