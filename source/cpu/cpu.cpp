#include <cstdint>
#include <iostream>
#include <vector>

#include "cpu/cpu.h"
#include "cpu/instruction.h"
#include "memory/ram.h"
#include "memory/register.h"

namespace cpu
{

CPU::CPU()
{
	opcode_table_[0x00] = &CPU::executeADD;
	opcode_table_[0x04] = &CPU::executeAND;
	opcode_table_[0x21] = &CPU::executeBEQ;
	opcode_table_[0x23] = &CPU::executeBGT;
	opcode_table_[0x24] = &CPU::executeBLT;
	opcode_table_[0x22] = &CPU::executeBNE;
	opcode_table_[0x30] = &CPU::executeHLT;
	opcode_table_[0x33] = &CPU::executeJMP;
	opcode_table_[0x20] = &CPU::executeJMPI;
	opcode_table_[0x02] = &CPU::executeLDA;
	opcode_table_[0x11] = &CPU::executeLLI;
	opcode_table_[0x10] = &CPU::executeLUI;
	opcode_table_[0x01] = &CPU::executeMOV;
	opcode_table_[0x31] = &CPU::executeNOP;
	opcode_table_[0x32] = &CPU::executeNOT;
	opcode_table_[0x03] = &CPU::executeOR;
	opcode_table_[0x05] = &CPU::executeSTA;
	opcode_table_[0x06] = &CPU::executeSUB;
	opcode_table_[0x07] = &CPU::executeXOR;
}



void CPU::loadProgram(const std::vector<uint16_t>& program, uint16_t start_addr)
{
	for (size_t i = 0; i < program.size(); ++i)
	{
		ram_.writeWord(program[i], start_addr + i * 2);
	}
}

Status CPU::getStatus()
{
	Status status{};
	status.pc = pc_.read();

	for (std::size_t i{0U}; i < registers_.size(); ++i)
	{
		status.registers[i] = registers_[i].read();
	}
	
	status.flags = flags_.read();
	status.halted = halted_;

	return status;
}

void CPU::reset()
{
	clearRegisters();
	pc_.reset();
	flags_.clear();
	halted_ = false;
}

void CPU::clearRegisters()
{
	for (memory::Register& reg :  registers_)
	{
		reg.clear();
	}
}

std::uint16_t CPU::getRegValue(std::string reg)
{

	std::uint16_t regIndex{0U}; 

	if (std::toupper(reg[0]) != 'R')
	{
		throw std::invalid_argument("Invalid argument for <reg>: Unrecognized register");
	}

	try { regIndex = std::stoul(reg.substr(1), nullptr , 0);}
	catch (const std::exception&) { throw std::invalid_argument("Invalid argument for <reg>: Unrecognized register"); }
	
	if (regIndex > REG_AMOUNT - 1)
	{
		throw std::invalid_argument("Invalid argument for <reg>: Register does not exist");
	}

	return registers_[regIndex].read();

}

std::uint16_t CPU::getProgramCounter()
{
	return pc_.read();
}

std::uint16_t CPU::readMemory(std::uint16_t address)
{
	return ram_.read(address);
}

std::vector<uint16_t> CPU::readMemoryRange(std::uint16_t startAddr, std::uint16_t endAddr)
{
	std::vector<uint16_t> memoryData{};

	if (startAddr > 0xFFFF or startAddr > 0xFFFF)
	{
		throw std::invalid_argument("Invalid argument: Start/End address out of bounds");
	}
	if (startAddr > endAddr){
		throw std::invalid_argument("Invalid argument: start address must be lower than end address");
	}

	for (std::size_t i = startAddr; i <= endAddr; ++i)
	{
		std::uint16_t memData = ram_.read(i);
		memoryData.push_back(memData);
	}

	return memoryData;

}

std::uint16_t CPU::fetch()
{
	std::uint16_t instruction = ram_.readWord(pc_.read());
	pc_.increment();
	return instruction;
}

Instruction CPU::decode(std::uint16_t instruction_bits)
{
	Instruction instruction;

	std::uint8_t format 	= instruction_bits >> 14U;
	std::uint8_t opcode 	= (instruction_bits >> 10U) & 0xF;

	instruction.id 			= (format << 4U) | opcode;
	instruction.raw			= instruction_bits;

	switch (format)
	{
		case TWO_REG_OP:
		{
			instruction.reg1 = static_cast<memory::Registers>((instruction_bits >> 4U) & 0xF);
			instruction.reg2 = static_cast<memory::Registers>(instruction_bits & 0xF);
			break;
		}
		case IMMEDIATE_OP:
		{
			instruction.reg1 		= static_cast<memory::Registers>((instruction_bits >> 8U) & 0x3);
			instruction.immediate	= instruction_bits & 0xFF;
			break;
		}
		case JUMP_OP:
		{
			instruction.reg1 		= static_cast<memory::Registers>((instruction_bits >> 4U) & 0xF);
			instruction.immediate 	= instruction_bits & 0x3FF;
			break;
		}
		case SINGLE_REG_OP:
		{
			instruction.reg1 = static_cast<memory::Registers>((instruction_bits >> 4U) & 0xF);
			break;
		}
	}
	
	return instruction;
}

void CPU::execute(Instruction instruction)
{
	if (opcode_table_[instruction.id])
	{
		(this->*opcode_table_[instruction.id])(instruction);

	}
}

void CPU::step()
{
	std::uint16_t instruction_bits = fetch();
	Instruction instruction = decode(instruction_bits);
	execute(instruction);
}

} // namespace cpu