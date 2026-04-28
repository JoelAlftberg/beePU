#pragma once

#include "cpu/cpu.h"
#include "utils.h"

#include <string>
#include <unordered_set>
#include <vector>



namespace cli
{

struct Input
{
	std::string command;
	std::vector<std::string> arguments;
};

struct Output
{
	std::vector<std::string> lines;
	bool shouldExit{false};
};

class CLI
{

public:
	Input readInput();
	Input readInput(const std::string& input);
	
	Output evaluateInput(const Input& input);
	void printOutput(const Output& output);

	bool shouldExit() { return shouldExit_; }

	Output handleRun(const std::vector<std::string>&);
	Output handlePause(const std::vector<std::string>&);
	Output handleStep(const std::vector<std::string>&);
	Output handleBreak(const std::vector<std::string>&);
	Output handleReset(const std::vector<std::string>&);
	Output handleGet(const std::vector<std::string>&);
	Output handleSet(const std::vector<std::string>&);
	Output handleStatus(const std::vector<std::string>&);
	Output handleHelp(const std::vector<std::string>&);
	Output handleExit(const std::vector<std::string>&);

	CLI(cpu::CPU& cpu);
	~CLI() = default;

private:
	cpu::CPU& cpu_;
	utils::DispatchTable<Output(const std::vector<std::string>&)> commandTable_;
	std::unordered_set<std::uint16_t> breakpoints_;
	bool shouldExit_;

};

} // namespace cli