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
			output.lines.push_back(std::format("Breakpoint hit @ 0x{:04x}", currentAddress));
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

	output.lines.push_back(std::format("Stepped (0x{:04x}) -> (0x{:04x})", startAddress ,cpu_.getProgramCounter()));

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
			output.lines.push_back(std::format("(0x{:04x})", address));
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
	return Output{};
}

Output CLI::handleSet(const std::vector<std::string>& arguments)
{
	return Output{};
}

Output CLI::handleStatus(const std::vector<std::string>& arguments)
{
	Output output{};

	cpu::Status status{cpu_.getStatus()};

	std::string state = status.halted ? "Halted" : "Ready";

	output.lines.push_back(std::format("CPU State: {}", state));
	output.lines.push_back(std::format("Flags: N={} Z={} C={} V={}", status.flags & 0x01, status.flags & 0x00, status.flags & 0x02, status.flags & 0x03));
	output.lines.push_back(std::format("PC @ (0x{:04X})", status.pc));
	for(std::size_t i{0U}; i < status.registers.size(); ++i)
	{
		output.lines.push_back(std::format("R{}: 0x{:04X}", i, status.registers[i]));
	}

	return output;
}

Output CLI::handleHelp(const std::vector<std::string>& arguments)
{
	return Output{};
}

Output CLI::handleExit(const std::vector<std::string>& arguments)
{
	Output output{};
	output.lines.push_back("Exiting");
	shouldExit_ = true;

	return output;
}

} // namespace cli