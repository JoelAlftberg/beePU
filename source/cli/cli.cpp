#include "cli/cli.h"
#include "cpu/cpu.h"
#include "memory/register.h"
#include "utils.h"

#include <cstdio>
#include <format>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace cli
{

CLI::CLI(cpu::CPU& cpu)
	: cpu_{cpu}
	, commandTable_{}
	, breakpoints_{}
	, shouldExit_{false}

{
	commandTable_["run"] 	= [this](const std::vector<std::string>& args){ return handleRun(args); };
	commandTable_["pause"] 	= [this](const std::vector<std::string>& args){ return handlePause(args); };
	commandTable_["step"] 	= [this](const std::vector<std::string>& args){ return handleStep(args); };
	commandTable_["break"] 	= [this](const std::vector<std::string>& args){ return handleBreak(args); };
	commandTable_["reset"] 	= [this](const std::vector<std::string>& args){ return handleReset(args); };
	commandTable_["get"] 	= [this](const std::vector<std::string>& args){ return handleGet(args); };
	commandTable_["set"] 	= [this](const std::vector<std::string>& args){ return handleSet(args); };
	commandTable_["status"] = [this](const std::vector<std::string>& args){ return handleStatus(args); };
	commandTable_["help"] 	= [this](const std::vector<std::string>& args){ return handleHelp(args); };
	commandTable_["exit"] 	= [this](const std::vector<std::string>& args){ return handleExit(args); };
}

Input CLI::readInput()
{
	std::string userInput;

	std::cout << "beePU> ";
	std::getline(std::cin, userInput);

	std::vector<std::string> tokens{utils::split(userInput)};
	
	if (tokens.empty()) { return Input{};}

	std::string command{tokens[0]};
	std::vector<std::string> args(tokens.begin() + 1, tokens.end());

	return Input{command, args};
}

Input CLI::readInput(const std::string& input)
{

	std::vector<std::string> tokens{utils::split(input)};
	
	if (tokens.empty()) { return Input{};}

	std::string command{tokens[0]};
	std::vector<std::string> args(tokens.begin() + 1, tokens.end());

	return Input{command, args};
}

Output CLI::evaluateInput(const Input& input)
{
	auto command = commandTable_.find(input.command);

	if (commandTable_.end() == command)
	{
		return Output{};
	}

	return command->second(input.arguments);
}

void CLI::printOutput(const Output& output)
{
	for (std::string line : output.lines)
	{
		std::cout << line << "\n";
	}
}

Output CLI::handleRun(const std::vector<std::string>& arguments)
{

	Output output{};
	
	while(true)
	{
		std::uint16_t currentAddress = cpu_.getProgramCounter();

		if (cpu_.halted())
		{
			output.lines.push_back(std::format("(0x{:04X}) Execution finished.", cpu_.getProgramCounter()));
			return output;
		}
		else
		{
			cpu_.step();
		}
		if (breakpoints_.end() != breakpoints_.find(currentAddress))
		{
			output.lines.push_back(std::format("Breakpoint hit @ 0x{:04X}", currentAddress));
			return output;
		}
	}
}

Output CLI::handlePause(const std::vector<std::string>& arguments)
{
	return Output{};
}

Output CLI::handleStep(const std::vector<std::string>& arguments)
{
	Output output{};
	std::uint16_t steps{0U};

	std::uint16_t startAddress = cpu_.getProgramCounter();

	if (cpu_.halted()) { output.lines.push_back("CPU is halted"); return output; }

	if (!arguments.empty())
	{
		try
		{
			steps = std::stoul(arguments[0]);

		}
		catch (const std::exception& e)
		{
			output.lines.push_back(e.what());
		}
	}
	else
	{
		steps = 1U;
	}

	for (;steps > 0; --steps)
	{
		cpu_.step();
		
	}

	output.lines.push_back(std::format("Stepped (0x{:04X}) -> (0x{:04X})", startAddress ,cpu_.getProgramCounter()));

	return output;
}

Output CLI::handleBreak(const std::vector<std::string>& arguments)
{

	Output output{};
	if (arguments.empty())
	{
		output.lines.push_back("Breakpoints:");
		for (uint16_t address : breakpoints_)
		{
			output.lines.push_back(std::format("(0x{:04X})", address));
		}	
	}
	else
	{
		for (std::string argument : arguments)
		{
			if ("clear" == argument)
			{
				breakpoints_.clear();
				output.lines.push_back("Breakpoints cleared");
				return output;
			}
			try
			{ 
				unsigned long address{std::stoul(argument, nullptr, 16)};
				if (address >= 0xFFFF)
				{
					output.lines.push_back(std::format("Address (0x{:04X}) out of bounds", address));
				}
				else
				{
					breakpoints_.insert(static_cast<uint16_t>(address));
					output.lines.push_back(std::format("Breakpoint set @ (0x{:04X})", address));
				}

			}
			catch (const std::exception& e)
			{ 
				output.lines.push_back(e.what()); 
			}
		}
	}

	return output;
}

Output CLI::handleReset(const std::vector<std::string>& arguments)
{
	cpu_.reset();
	return Output{{"CPU Reset"}};
}

Output CLI::handleGet(const std::vector<std::string>& arguments)
{
	Output output{};

	if (arguments.empty())
	{
		return handleHelp({"get"});
	}
	
	if ("reg" == arguments[0])
	{
		if (arguments.size() < 2)
		{
			for(std::uint8_t i = 0; i < REG_AMOUNT; ++i)
			{
				std::uint16_t regValue{0U};
				std::string regIndex = std::format("R{}", i);
				try {
					regValue = cpu_.getRegValue(regIndex);
				}
				catch (const std::exception& e)
				{
					output.lines.push_back(e.what());
					return output;
				}
				output.lines.push_back(std::format("{}: 0x{:04X}", regIndex, regValue));
			}
		}
		else
		{
			std::uint16_t regValue{0U};
			std::string reg = arguments[1];

			try {
				regValue = cpu_.getRegValue(reg);
			}
			catch (const std::exception& e)
			{
				output.lines.push_back(e.what());
				return output;
			}

			output.lines.push_back(std::format("{}: 0x{:04X}", arguments[1], regValue));
		}
		
	}
	else if ("mem" == arguments[0])
	{
		std::vector<std::uint16_t> memoryData{};

		if(2U == arguments.size())
		{
			try
			{
				std::uint16_t address = std::stoul(arguments[1], nullptr, 0);
				for(int i{address}; i < (address + 18); ++i)
				{
					output.lines.push_back(std::format("(0x{:04X}):	{:02X}", i ,cpu_.readMemory(i)));
				}

			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			
		}
		if (arguments.size() > 2)
		{
			try
			{
				std::uint16_t startAddr = std::stoul(arguments[1], nullptr, 0);
				std::uint16_t endAddr = std::stoul(arguments[2], nullptr, 0);
				memoryData = cpu_.readMemoryRange(startAddr, endAddr);
				for (std::size_t i = 0; i < memoryData.size(); ++i)
				{
					output.lines.push_back(std::format("(0x{:04X}):	0x{:02X}", startAddr++, memoryData[i]));
				}
			}
			catch(const std::exception& e)
			{
				output.lines.push_back(e.what());
			}
		}

	}

	return output;
}

Output CLI::handleSet(const std::vector<std::string>& arguments)
{
	Output output{};

	if (arguments.empty())
	{
		return handleHelp({"set"});
	}

	if ("reg" == arguments[0])
	{
	}

	else if ("mem" == arguments[0])
	{

	}
	return output;
}

Output CLI::handleStatus(const std::vector<std::string>& arguments)
{
	Output output{};

	cpu::Status status{cpu_.getStatus()};

	std::string state = status.halted ? "Halted" : "Ready";

	output.lines.push_back(std::format("CPU State: {}", state));
	
	std::uint8_t flagN = (status.flags & 0x02) != 0U ? 1U : 0U;
	std::uint8_t flagZ = (status.flags & 0x01) != 0U ? 1U : 0U;
	std::uint8_t flagC = (status.flags & 0x04) != 0U ? 1U : 0U;
	std::uint8_t flagV = (status.flags & 0x08) != 0U ? 1U : 0U;

	output.lines.push_back(std::format("N={} Z={} C={} V={}", flagN, flagZ, flagC, flagV));
	output.lines.push_back(std::format("PC: 0x{:04X}", status.pc));
	for(std::size_t i{0U}; i < status.registers.size(); ++i)
	{
		output.lines.push_back(std::format("R{}: 0x{:04X}", i, status.registers[i]));
	}

	return output;
}

Output CLI::handleHelp(const std::vector<std::string>& arguments)
{
	Output output{};

	if (arguments.empty())
	{
		output.lines.push_back("Usage: help <command> for command usage instructions");
		output.lines.push_back("run 	-  Run the loaded binary");
		output.lines.push_back("step 	-  Step the loaded binary");
		output.lines.push_back("break	-  Set breakpoints");
		output.lines.push_back("reset 	-  Reset the CPU");
		output.lines.push_back("get 	-  Get data from registers/memory");
		output.lines.push_back("set 	-  Set data in of register/memory");
		output.lines.push_back("status 	-  Get the status of the CPU");
		output.lines.push_back("exit 	-  Exit the emulator");
	}
	
	else if ("break" == arguments[0])
	{
		output.lines.push_back("Usage: break <addr> <...>");
		output.lines.push_back("	Set breakpoints at one or more specified addresses");
		output.lines.push_back("	Running the command without any address specified lists all breakpoints");
	}
	else if ("step" == arguments[0])
	{
		output.lines.push_back("Usage: step <n>");
		output.lines.push_back("	Steps the binary <n> amount of times");
		output.lines.push_back("	Running the command without any arguments will step the program once");
	}
	else if ("reset" == arguments[0])
	{
		output.lines.push_back("Usage: reset");
		output.lines.push_back("	Resets all registers, flags and the PC of the CPU");
		output.lines.push_back("	Note: Does not reset breakpoints or RAM");
	}
	else if ("status" == arguments[0])
	{
		output.lines.push_back("Usage: status");
		output.lines.push_back("	Shows the status of all the PC, flags and registers and the CPU state");
	}
	else if ("get" == arguments[0])
	{
		output.lines.push_back("Usage: get <submodule> [<arg>] [<...>]");
		output.lines.push_back("	Gets data from the submodule specified");
		output.lines.push_back("	Available submodules");
		output.lines.push_back("	* mem - For detailed usage run 'help get mem'");
		output.lines.push_back("	* reg - For detailed usage run 'help get reg'");
		if (arguments.size() > 1 && "mem" == arguments[1])
		{
			output.lines.clear();
			output.lines.push_back("");
			output.lines.push_back("Description:");
			output.lines.push_back(" Gets data from memory");
			output.lines.push_back("");
			output.lines.push_back("Usage:");
			output.lines.push_back(" get mem			- Dumps all available memory");
			output.lines.push_back(" get mem <addr>			- Dumps data at specific address");
			output.lines.push_back(" get mem <start> <end>		- Dumps a range of memory");
			output.lines.push_back("");
			output.lines.push_back("Options:");
			output.lines.push_back(" <addr>		Hexadecimal address (e.g., 0x014F)");
			output.lines.push_back(" <start>	Beginning of the range");
			output.lines.push_back(" <end>		End of the range (inclusive)");
		}
		if (arguments.size() > 1 && "reg" == arguments[1])
		{
			output.lines.clear();
			output.lines.push_back("");
			output.lines.push_back("Description:");
			output.lines.push_back(" Gets values from registers");
			output.lines.push_back("");
			output.lines.push_back("Usage:");
			output.lines.push_back(" get reg 	- Gets values of all registers");
			output.lines.push_back(" get reg <reg> 	- Gets the value of a specified register");
			output.lines.push_back("");
			output.lines.push_back("Options:");
			output.lines.push_back(" <reg>		Register");
		}
	}
	else if ("set" == arguments[0])
	{
		output.lines.push_back("Usage: set <submodule> [<arg>] [<...>]");
		output.lines.push_back("	Sets data for the submodule specified");
		output.lines.push_back("	Available submodules");
		output.lines.push_back("	* mem - For detailed usage run 'help set mem'");
		output.lines.push_back("	* reg - For detailed usage run 'help set reg'");
		if (arguments.size() > 1 && "mem" == arguments[1])
		{
			output.lines.clear();
			output.lines.push_back("");
			output.lines.push_back("Description:");
			output.lines.push_back(" Sets data at an address in memory");
			output.lines.push_back("");
			output.lines.push_back("Usage:");
			output.lines.push_back(" set mem <addr> <data>	- Sets the data at a specified address");
			output.lines.push_back("");
			output.lines.push_back("Options:");
			output.lines.push_back(" <addr>		Hexadecimal address (e.g., 0x014F)");
			output.lines.push_back(" <data>		Data to store");


		}
		if (arguments.size() > 1 && "reg" == arguments[1])
		{
			output.lines.clear();
			output.lines.push_back("");
			output.lines.push_back("Description:");
			output.lines.push_back(" Sets the value of a registers");
			output.lines.push_back("");
			output.lines.push_back("Usage:");
			output.lines.push_back(" set reg <reg> <value>	- Sets the value of a specified registered");
			output.lines.push_back("");
			output.lines.push_back("Options:");
			output.lines.push_back(" <reg>		Register");
			output.lines.push_back(" <value>	Value to store");
		}
	}

	output.lines.push_back("");
	return output;
}

Output CLI::handleExit(const std::vector<std::string>& arguments)
{
	Output output{};
	shouldExit_ = true;

	return output;
}

} // namespace cli