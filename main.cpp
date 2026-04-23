#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "cli/cli.h"
#include "cpu/cpu.h"
#include "memory/ram.h"

int main(int argc, char* argv[])
{
	
	cpu::CPU cpu{};
	cli::CLI cli{cpu};

	const char* inputBinary = argv[1];

	if (argc < 2)
	{
		std::cout << "Usage: beePU <program.bin>" << "\n";
		return 1;
	}
	
	if (!std::filesystem::is_regular_file(inputBinary)) 
	{ 
		std::cout << "ERROR: File \"" << inputBinary << "\" not found" << "\n"; 
		return 1; 
	}

	std::ifstream inputStream(inputBinary, std::ios::binary);
	std::vector<uint16_t> program;
	
	std::uint16_t word;

	while(inputStream.read(reinterpret_cast<char*>(&word), sizeof(uint16_t)))
	{
		program.push_back(word);
	}

	cpu.loadProgram(program, 0);


	while(1)
	{
		cli::Input input{cli.readInput()};
		cli::Output output{cli.evaluateInput(input)};
		cli.printOutput(output);
	}

}