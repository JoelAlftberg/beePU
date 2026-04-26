#include "arch.h"
#include "assembler.h"
#include "utils.h"

#include <bitset>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>

Assembler::Assembler(const char* output, const char* format)
	: outputFile_(output)
	, outputFormat_(format)
{
	lookupTable_["ADD"] 	= { 0b00, 0b0000};
	lookupTable_["AND"] 	= { 0b00, 0b0100};
	lookupTable_["BEQ"] 	= { 0b10, 0b0001};
	lookupTable_["BGT"] 	= { 0b10, 0b0011};
	lookupTable_["BLT"] 	= { 0b10, 0b0100};
	lookupTable_["BNE"] 	= { 0b10, 0b0010};
	lookupTable_["BANK"] 	= { 0b11, 0b0100};
	lookupTable_["CALL"] 	= { 0b11, 0b0111};
	lookupTable_["HLT"] 	= { 0b11, 0b0000};
	lookupTable_["JMP"] 	= { 0b11, 0b0011};
	lookupTable_["JMPI"] 	= { 0b10, 0b0000};
	lookupTable_["LDA"] 	= { 0b00, 0b0010};
	lookupTable_["LLI"] 	= { 0b01, 0b0001};
	lookupTable_["LUI"] 	= { 0b01, 0b0000};
	lookupTable_["MOV"] 	= { 0b00, 0b0001};
	lookupTable_["NOP"] 	= { 0b11, 0b0001};
	lookupTable_["NOT"] 	= { 0b11, 0b0010};
	lookupTable_["OR"] 		= { 0b00, 0b0011};
	lookupTable_["POP"] 	= { 0b11, 0b0101};
	lookupTable_["PUSH"] 	= { 0b11, 0b0110};
	lookupTable_["RET"] 	= { 0b11, 0b1000};
	lookupTable_["STA"] 	= { 0b00, 0b0101};
	lookupTable_["SUB"] 	= { 0b00, 0b0110};
	lookupTable_["XOR"] 	= { 0b00, 0b0111};
}

void Assembler::firstPass(std::string& line)
{

	if (std::string::npos == line.find_first_not_of(" \t"))
	{
		return;
	}

	line = line.substr(line.find_first_not_of(" \t"));

	size_t origin_index = line.find(".org");
	size_t label_index = line.find(':');
	size_t comment_index = line.find(';');

	if (std::string::npos != comment_index)
	{
		line = line.substr(0, comment_index);
		if (std::string::npos == line.find_first_not_of(" \t"))
		{
			return;
		}
	}

	if (std::string::npos != label_index)
	{
		std::string label = line.substr(0, label_index);
		symbols_[label] = currentAddress_;
		return;
	}

	if (std::string::npos != origin_index)
	{
		originAddress_ = static_cast<std::uint16_t>(std::stoul(line.substr(origin_index + 4), nullptr, 0U));
		currentAddress_ = originAddress_;
		return;
	}

	lines_.push_back(line);
	
	std::vector<std::string> tokens = utils::split(line);

	if (tokens.size() > 1)
	{
		if ("CALL" == tokens[0] and 'R' != std::toupper(tokens[1][0]))
		{
			currentAddress_ += (INSTRUCTION_WIDTH * 2);
		}
	}

	currentAddress_ += INSTRUCTION_WIDTH;
}

void Assembler::secondPass()
{

	currentAddress_ = originAddress_;

	for (const std::string& line : lines_)
	{
		std::vector<std::string> tokens = utils::split(line);
		std::size_t tokenAmount = tokens.size();

		if (tokens.empty())
		{
			continue;
		}
		
		if (tokenAmount > 3)
		{
			std::cout << "Line '" << line << "' contains more than 3 tokens, check syntax" << "\n";
		}

		std::string mnemonic = tokens[0];
		if(!lookupTable_.contains(mnemonic)){
			std::cout << "Unknown mnemonic '" << mnemonic << "' found, discarding." << "\n";
			continue;
		}
		
		std::uint16_t binaryRepr{0U};
		std::uint8_t format = lookupTable_[mnemonic].format;
		std::uint8_t opcode = lookupTable_[mnemonic].opcode;
		std::uint16_t instructionId = (format << 4) | opcode;
		binaryRepr |= (instructionId << OPCODE_SHIFT);

		switch (format)
		{
			case TWO_REG_OP:
			{
				if (tokenAmount < 3) 
				{	
					std::cout << "Too few tokens found for a two-registers operation" << "\n";
					continue;
				}
				
				std::uint8_t src =  std::stoul(tokens[1].substr(1));
				std::uint8_t dst =  std::stoul(tokens[2].substr(1));
				binaryRepr |= (src << SRC_SHIFT) | dst;
				break;
			}
			case IMMEDIATE_OP:
			{
				if (tokenAmount < 3)
				{	
					std::cout << "Too few tokens found for an operation" << "\n";
					continue;
				}

				std::uint8_t dst =  std::stoul(tokens[1].substr(1));
				std::uint16_t immediate = std::stoul(tokens[2], nullptr, 0U);
				binaryRepr |= (dst << BYTE_WIDTH) | immediate;
				break;
			}
			case JUMP_OP:
			{
				if (tokenAmount < 2)
				{	
					std::cout << "Too few tokens found for a jump operation" << "\n";
					continue;
				}

				auto iter = symbols_.find(tokens[1]);

				if (iter != symbols_.end())
				{
					std::cout << "Label: " << iter->first << "\n" << "Address: " << std::format("0x{:04X}", iter->second) << "\n";
					std::uint16_t labelAddress = iter->second;
					std::uint16_t offset = labelAddress - (currentAddress_ + INSTRUCTION_WIDTH);
					std::cout << "Offset: " << (int16_t)offset << "\n";

					binaryRepr |= (offset & 0x3FF);
				}

				else
				{
					std::uint16_t immediate = std::stoul(tokens[1], nullptr, 0U);
					binaryRepr |= immediate;
				}

				break;
			}
			case SINGLE_REG_OP:
			{
				if (tokenAmount > 1)
				{
					if ("CALL" == mnemonic and symbols_.end() != symbols_.find(tokens[1]))
					{
						emitCallLabel(tokens);
						binaryRepr |= (SCRATCH_REGISTER << SRC_SHIFT);
					}
					else
					{
						std::uint8_t src =  std::stoul(tokens[1].substr(1));
						binaryRepr |= (src << SRC_SHIFT);
					} 
				}
				break;
			}
		}

		currentAddress_ += INSTRUCTION_WIDTH;
		outputBuf_.push_back(binaryRepr);

	}
}

void Assembler::emitCallLabel(std::vector<std::string> tokens)
{
	std::uint16_t labelAddress = symbols_[tokens[1]];
	std::cout << "Label: " << tokens[1] << "\n" << "Address: " << std::format("0x{:04X}", (int)labelAddress) << "\n";
	std::uint16_t instructionId{0U};
	std::uint8_t format{0U};
	std::uint8_t opcode{0U};

	std::uint8_t lowerAddrByte = (labelAddress & LOWER_BYTE_MASK);
	format = lookupTable_["LLI"].format;
	opcode = lookupTable_["LLI"].opcode;
	instructionId = (format << 4) | opcode;
	std::uint16_t loadLowerAddr = (instructionId << OPCODE_SHIFT) | lowerAddrByte;
	loadLowerAddr |= (SCRATCH_REGISTER << BYTE_WIDTH);
	outputBuf_.push_back(loadLowerAddr);

	currentAddress_ += INSTRUCTION_WIDTH;

	std::uint8_t upperAddrByte = (labelAddress >> BYTE_WIDTH);
	format = lookupTable_["LUI"].format;
	opcode = lookupTable_["LUI"].opcode;
	instructionId = (format << 4) | opcode;
	std::uint16_t loadUpperAddr = (instructionId << OPCODE_SHIFT) | upperAddrByte;
	loadUpperAddr |= (SCRATCH_REGISTER << BYTE_WIDTH);
	outputBuf_.push_back(loadUpperAddr);

	currentAddress_ += INSTRUCTION_WIDTH;
}

void Assembler::writeOutput()
{
	std::ofstream out(outputFile_ ,std::ios::binary);
	if (!out.is_open())
	{
		std::cout << "ERROR: Couldn't open output file '" << outputFile_ << "'" << "\n";
	}

	if (outputFormat_ == "hex") 
	{ 
		for (std::uint16_t word : outputBuf_) { out << "0x" << std::setw(4) << std::setfill('0') << std::hex << word << "\n"; }
	}

	else if (outputFormat_ == "bin")
	{
		for (std::uint16_t word : outputBuf_) { out.write(reinterpret_cast<const char*>(&word), sizeof(std::uint16_t)); }
	}

	else if (outputFormat_ == "bin_text")
	{
		for (std::uint16_t word : outputBuf_) { out << "0b" << std::bitset<16>(word) << "\n"; }
	}

	else
	{
		std::cout << "ERROR: Invalid format specified, should be either 'bin' or 'hex'" << "\n";
		std::filesystem::remove(outputFile_);
	}
}