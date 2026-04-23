#include "cli/cli.h"
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
{
	commandTable_["run"] 	= [this](const std::vector<std::string>& args){ return handleRun(args); };
	commandTable_["pause"] 	= [this](const std::vector<std::string>& args){ return handlePause(args); };
	commandTable_["step"] 	= [this](const std::vector<std::string>& args){ return handleStep(args); };
	commandTable_["break"] 	= [this](const std::vector<std::string>& args){ return handleBreak(args); };
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
		cpu_.step();
		if (cpu_.halted())
		{
			output.lines.push_back("Execution finished.");
			return output;
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
	return Output{};
}

Output CLI::handleBreak(const std::vector<std::string>& arguments)
{
	return Output{};
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
	return Output{};
}

Output CLI::handleHelp(const std::vector<std::string>& arguments)
{
	return Output{};
}

Output CLI::handleExit(const std::vector<std::string>& arguments)
{
	return Output{};
}

} // namespace cli