#include <iostream>
#include <vector>

#include "cpu/cpu.h"
#include "memory/ram.h"

int main()
{
	cpu::CPU cpu{};
	

	std::vector<uint16_t> program = {
		0b0100010101001000,		// LLI 01 0001, r1, immediate 72
		0b0000010000010110,		// MOV 00 0000, r1, r2
		0b0000000000010110,		// ADD 00 0001, r1, r2
		0b1100000000000000,		// HLT 11 0000
	};

	cpu.loadProgram(program, 0);

	while(!cpu.halted())
	{
		std::cin.get();
		cpu.step();
		if (cpu.halted()){ return 0;}
		cpu.printState();

	}

}