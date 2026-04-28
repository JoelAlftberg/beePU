#pragma once

#include "cli/cli.h"
#include "cpu/cpu.h"
#include "tui/debugger.h"

#include <ncurses.h>

#include <format>
#include <string>
#include <vector>


namespace tui
{

class MemoryPane
{

public:

	MemoryPane(WINDOW* memoryWindow, cpu::CPU& cpu)
		: memoryWindow_{memoryWindow}
		, memoryPad_{newpad(0xFFFF, RIGHT_PANE_COLS - 2)}
		, cli_{cpu}
	{
	}

	void tick(Debugger::Output memoryViewPos)
	{
		previousData_ = currentData_;
		
		werase(memoryWindow_);
		box(memoryWindow_, 0, 0);
		mvwprintw(memoryWindow_, 0, 2, "Memory:");

		if (memoryViewPos.memoryArg.has_value())
		{
			memoryPadOffset_ = memoryViewPos.memoryArg.value();
		}

		currentData_ = getMemory(memoryPadOffset_);
	}

	std::vector<std::string> getMemory(std::string memoryOffset)
	{
		std::uint16_t row{1U};
		std::vector<std::string> allMemoryCommand{"mem", memoryPadOffset_};
		cli::Output output{cli_.handleGet(allMemoryCommand)};

		for (std::string line : output.lines)
		{
			mvwaddnstr(memoryWindow_, row, 1U, line.c_str(), RIGHT_PANE_COLS);
			++row;
		}
	
		return output.lines;
	}
	

private:
	WINDOW* memoryWindow_;
	WINDOW* memoryPad_;
	cli::CLI cli_;
	std::string memoryPadOffset_{"0x0000"};
	std::vector<std::string> currentData_{};
	std::vector<std::string> previousData_{};

};

} // namespace tui
