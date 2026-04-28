#pragma once

#include "cli/cli.h"
#include "cpu/cpu.h"

//#include "ncurses/ncurses.h"
#include <ncurses.h>

#include <vector>

namespace tui
{

class RegisterPane
{

public:

	RegisterPane(WINDOW* registerWindow, cpu::CPU& cpu)
		: registerWindow_{registerWindow}
		, cli_{cpu}
	{}

	void tick()
	{
		werase(registerWindow_);
		box(registerWindow_, 0, 0);
		mvwprintw(registerWindow_, 0, 2, "State:");

		cli::Output output{cli_.handleStatus({})};
		printOutput(output.lines);
	}

	void printOutput(std::vector<std::string> lines)
	{
		std::uint8_t maxLines{static_cast<uint8_t>(getmaxy(registerWindow_) - 1)};
		std::uint8_t row{1U};

		for (std::string line : lines)
		{
			if(row >= maxLines) { return; }
			mvwaddnstr(registerWindow_, row, 1U, line.c_str(), RIGHT_PANE_COLS);
			row++;
		}
	}

private:
	WINDOW* registerWindow_;
	cli::CLI cli_;
	std::vector<std::string> previousState_{};

};
	
} // namespace tui
