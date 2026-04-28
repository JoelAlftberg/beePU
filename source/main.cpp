#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "cli/cli.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

#include "tui/screen.h"
#include "tui/debugger.h"

int main(int argc, char* argv[])
{

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
	std::vector<uint16_t> programBinary;
	
	std::uint16_t word;

	while(inputStream.read(reinterpret_cast<char*>(&word), sizeof(uint16_t)))
	{
		programBinary.push_back(word);
	}

	cpu::CPU cpu{programBinary};
	cpu.loadProgram(programBinary, 0);
	
	tui::Screen screen{cpu};


	/*
	while(!cli.shouldExit())
	{
		cli::Input input{cli.readInput()};
		cli::Output output{cli.evaluateInput(input)};
		cli.printOutput(output);
	}
	*/

	while(1)
	{
		screen.update();
		screen.tick();

	}


	return 0;

}