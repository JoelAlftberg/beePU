#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "cpu/cpu.h"
#include "memory/ram.h"

int main(int argc, char* argv[])
{
	cpu::CPU cpu{};

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

	while(!cpu.halted())
	{
		std::cin.get();
		cpu.step();
		if (cpu.halted()){ return 0;}
		cpu.printState();

	}

}