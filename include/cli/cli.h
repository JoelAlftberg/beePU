#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "cpu/cpu.h"
#include "utils.h"

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
	Output evaluateInput(const Input& input);
	void printOutput(const Output& output);

	Output handleRun(const std::vector<std::string>&);
	Output handlePause(const std::vector<std::string>&);
	Output handleStep(const std::vector<std::string>&);
	Output handleBreak(const std::vector<std::string>&);
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

};

} // namespace cli