#include"cpu/cpu.h"

namespace cpu
{

void CPU::executeADD(const Instruction& instruction)
{
	memory::Register& src = registers_[static_cast<uint8_t>(instruction.reg1)];
	memory::Register& dst = registers_[static_cast<uint8_t>(instruction.reg2)];
	std::uint32_t sum = static_cast<uint32_t>(dst.read()) + static_cast<uint32_t>(src.read());

	flags_.clear();

	if (sum > 0xFFFF) {
		flags_.setFlag(Flag::Carry);
		flags_.setFlag(Flag::Overflow); 
	}
	if (sum & 0x8000) { flags_.setFlag(Flag::Negative); }
	if (0U == (sum & 0xFFFF)) { flags_.setFlag(Flag::Zero); }

	dst.write(static_cast<std::uint16_t>(sum));
}

void CPU::executeAND(const Instruction& instruction)
{
	memory::Register& src = registers_[static_cast<uint8_t>(instruction.reg1)];
	memory::Register& dst = registers_[static_cast<uint8_t>(instruction.reg2)];

	std::uint16_t result = src.read() & dst.read(); 
	
	if (result & 0x8000) { flags_.setFlag(Flag::Negative); }
	if (0U == (result & 0xFFFF)) { flags_.setFlag(Flag::Zero); }
	
	dst.write(result);
}

void CPU::executeBEQ(const Instruction& instruction)
{
	if(flags_.checkFlag(Flag::Zero))
	{
		std::int16_t offset = (instruction.immediate & 0x200) ? instruction.immediate | 0xFC00 : instruction.immediate;
		pc_.branch(offset);
	}
}

void CPU::executeBGT(const Instruction& instruction)
{
	if (!flags_.checkFlag(Flag::Zero) and !flags_.checkFlag(Flag::Negative))
	{
		std::int16_t offset = (instruction.immediate & 0x200) ? instruction.immediate | 0xFC00 : instruction.immediate;
		pc_.branch(offset);	
	}
}

void CPU::executeBLT(const Instruction& instruction)
{
	if (flags_.checkFlag(Flag::Negative))
	{
		std::int16_t offset = (instruction.immediate & 0x200) ? instruction.immediate | 0xFC00 : instruction.immediate;
		pc_.branch(offset);	
	}
}

void CPU::executeBNE(const Instruction& instruction)
{
	if (!flags_.checkFlag(Flag::Zero))
	{
		std::int16_t offset = (instruction.immediate & 0x200) ? instruction.immediate | 0xFC00 : instruction.immediate;
		pc_.branch(offset);	
	}
}

void CPU::executeBNK(const Instruction& instruction)
{
	std::uint8_t bankIndex = registers_[BANK_REGISTER].read();
	memController_.switchBank(bankIndex);
}

void CPU::executeHLT(const Instruction& instruction)
{
	halted_ = true;
}

void CPU::executeJMP(const Instruction& instruction)
{
	memory::Register& addr_reg = registers_[static_cast<uint8_t>(instruction.reg1)];
	std::uint16_t address = addr_reg.read();
	pc_.write(address);
}

void CPU::executeJMPI(const Instruction& instruction)
{
	std::int16_t offset = (instruction.immediate & 0x200) ? instruction.immediate | 0xFC00 : instruction.immediate;
	pc_.branch(offset);	
}

void CPU::executeLDA(const Instruction& instruction)
{
	memory::Register& src = registers_[static_cast<uint8_t>(instruction.reg1)];
	memory::Register& dst = registers_[static_cast<uint8_t>(instruction.reg2)];

	std::uint16_t addr = src.read();
	std::uint16_t data = memController_.readWord(addr);

	dst.write(data);
}

void CPU::executeLLI(const Instruction& instruction)
{
	memory::Register& dst = registers_[static_cast<std::uint8_t>(instruction.reg1)];
	dst.write(instruction.immediate);
}

void CPU::executeLUI(const Instruction& instruction)
{
	memory::Register& dst = registers_[static_cast<std::uint8_t>(instruction.reg1)];
	dst.write((dst.read() & LOWER_BYTE_MASK) | (instruction.immediate << BYTE_WIDTH));
}

void CPU::executeMOV(const Instruction& instruction)
{
	memory::Register& src = registers_[static_cast<uint8_t>(instruction.reg1)];
	memory::Register& dst = registers_[static_cast<uint8_t>(instruction.reg2)];

	dst.write(src.read());
}

void CPU::executeNOP(const Instruction& instruction){}

void CPU::executeNOT(const Instruction& instruction)
{
	memory::Register& src = registers_[static_cast<uint8_t>(instruction.reg1)];
	std::uint16_t inverted = ~(src.read());

	if (inverted & 0x8000) { flags_.setFlag(Flag::Negative); }
	if (0U == (inverted & 0xFFFF)) { flags_.setFlag(Flag::Zero); }

	src.write(inverted);
}
void CPU::executeOR(const Instruction& instruction)
{
	memory::Register& src = registers_[static_cast<uint8_t>(instruction.reg1)];
	memory::Register& dst = registers_[static_cast<uint8_t>(instruction.reg2)];

	std::uint16_t result = src.read() | dst.read(); 
	
	if (result & 0x8000) { flags_.setFlag(Flag::Negative); }
	if (0U == (result & 0xFFFF)) { flags_.setFlag(Flag::Zero); }
	
	dst.write(result);
}

void CPU::executeSTA(const Instruction& instruction)
{
	memory::Register& src = registers_[static_cast<uint8_t>(instruction.reg1)];
	memory::Register& dst = registers_[static_cast<uint8_t>(instruction.reg2)];

	std::uint16_t data = src.read();
	std::uint16_t addr = dst.read();

	memController_.writeWord(data, addr);
}

void CPU::executeSUB(const Instruction& instruction)
{
	memory::Register& src = registers_[static_cast<uint8_t>(instruction.reg1)];
	memory::Register& dst = registers_[static_cast<uint8_t>(instruction.reg2)];
	std::uint32_t diff = dst.read() - src.read();

	flags_.clear();
	if (src.read() > dst.read()) {
		flags_.setFlag(Flag::Carry);
		flags_.setFlag(Flag::Overflow); 
	}
	if (diff & 0x8000) { flags_.setFlag(Flag::Negative); }
	if (0U == (diff & 0xFFFF)) { flags_.setFlag(Flag::Zero); }

	dst.write(static_cast<std::uint16_t>(diff));
}

void CPU::executeXOR(const Instruction& instruction)
{
	memory::Register& src = registers_[static_cast<uint8_t>(instruction.reg1)];
	memory::Register& dst = registers_[static_cast<uint8_t>(instruction.reg2)];

	std::uint16_t result = src.read() ^ dst.read(); 
	
	if (result & 0x8000) { flags_.setFlag(Flag::Negative); }
	if (0U == (result & 0xFFFF)) { flags_.setFlag(Flag::Zero); }
	
	dst.write(result);
}

} // namespace cpu