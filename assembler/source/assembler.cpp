#include "arch.h"
#include "assembler.h"

#include <iostream>
#include <string>
#include <unordered_map>

Assembler::Assembler()
{
	lookupTable_["ADD"] = { 0b00, 0b0000};
	lookupTable_["AND"] = { 0b00, 0b0100};
	lookupTable_["BEQ"] = { 0b10, 0b0001};
	lookupTable_["BGT"] = { 0b10, 0b0011};
	lookupTable_["BLT"] = { 0b10, 0b0100};
	lookupTable_["BNE"] = { 0b10, 0b0010};
	lookupTable_["HLT"] = { 0b11, 0b0000};
	lookupTable_["JMP"] = { 0b10, 0b0000};
	lookupTable_["LDA"] = { 0b00, 0b0010};
	lookupTable_["LLI"] = { 0b01, 0b0001};
	lookupTable_["LUI"] = { 0b01, 0b0000};
	lookupTable_["MOV"] = { 0b00, 0b0001};
	lookupTable_["NOP"] = { 0b11, 0b0001};
	lookupTable_["NOT"] = { 0b11, 0b0010};
	lookupTable_["OR"] 	= { 0b00, 0b0011};
	lookupTable_["STA"] = { 0b00, 0b0101};
	lookupTable_["SUB"] = { 0b00, 0b0110};
	lookupTable_["XOR"] = { 0b00, 0b0111};
}

void Assembler::firstPass(std::string line)
{
	line = line.substr(line.find_first_not_of(" \t"));

	size_t origin_index = line.find(".org");
	size_t label_index = line.find(':');
	size_t comment_index = line.find(';');

	if (std::string::npos != comment_index)
	{
		line = line.substr(0, comment_index);
	}

	if (std::string::npos != label_index)
	{
		std::string label = line.substr(0, label_index);
		symbols_[label] = currentAddress_;
		return;
	}

	if (std::string::npos != origin_index)
	{
		currentAddress_ = static_cast<std::uint16_t>(std::stoul(line.substr(origin_index + 4), nullptr, 0U));
		return;
	}

	if (line.empty())
	{
		return;
	}

	lines_.push_back(line);
	currentAddress_ += INSTRUCTION_WIDTH;
}

void Assembler::secondPass()
{
	for (const std::string& line : lines_)
	{
		std::string mnemonic = line.substr(0, line.find_first_not_of(" \t"));
		if(!lookupTable_.contains(mnemonic)){
			std::cout << "Unknown mnemonic '" << mnemonic << "' found, discarding." << "\n";
			continue;
		}
		
		std::uint16_t binaryRepr{0U};

		// TODO: do lookup of mnemonic binary representation

	}
}