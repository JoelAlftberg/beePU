#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

struct MnemonicInfo {
	std::uint8_t format; 
	std::uint8_t opcode;
};

using MnemonicTable = std::unordered_map<std::string, MnemonicInfo>;

class Assembler
{

public:

	void firstPass(std::string& line);
	void secondPass();
	void writeOutput();

	Assembler(const char*, const char*);

	~Assembler() = default;

private:
	std::string outputFile_{};
	std::string outputFormat_{};
	std::unordered_map<std::string, std::uint16_t> symbols_{};
	std::vector<std::string> lines_{};
	std::vector<uint16_t> outputBuf_{};
	std::uint16_t currentAddress_{0U};
	MnemonicTable lookupTable_{};

};