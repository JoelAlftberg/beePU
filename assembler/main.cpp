#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "assembler.h"

int main(int argc, char* argv[])
{

	const char* output = nullptr;
	const char* input = nullptr;
	const char* format = nullptr;

	for (int i = 1; i < argc; ++i)
	{
		switch(*argv[i])
		{
			case '-':
			{
				if('o' == argv[i][1])
				{
					output = argv[i++ + 1];
					
				}
				else if ('f' == argv[i][1])
				{
					format = argv[i++ + 1];
				}
				break;
			}
			default:
			{
				input = argv[i];
				break;
			}
		}
	}

	if(nullptr == input) {
		std::cout << "ERROR: No source file specified" << "\n";
		return 1;
	}

	if (!std::filesystem::is_regular_file(input)) { 
		std::cout << "ERROR: File \"" << input << "\" not found" << "\n"; 
		return 1; 
	}

	std::ifstream source_file(input);
	std::string line;

	Assembler assembler{output, format};

	while (std::getline(source_file, line))
	{
		assembler.firstPass(line);
	}

	assembler.secondPass();
	assembler.writeOutput();
	
	return 0;
}