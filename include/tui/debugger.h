#pragma once

#include "arch.h"
#include "cli/cli.h"
#include "cpu/cpu.h"
#include "utils.h"

//#include "ncurses/ncurses.h"
#include <ncurses.h>

#include <string>
#include <vector>

namespace tui
{

static constexpr std::uint8_t DEBUG_WINDOW_COLS{DISPLAY_PANE_COLS - 6U};

class Debugger
{

public:

	Debugger(WINDOW* window, cpu::CPU& cpu)
		: debugWindow_{window}
		, cli_{cpu}
	{}

	void tick()
	{
		readInput();
		std::string input = inputBuf;
		Input input{cli_.readInput(input)};
		Output output{cli_.evaluateInput(Input)};

	}

	void resetPrompt()
	{
		mvwprintw(debugWindow_, 1, 1, "> ");
		wclrtoeol(debugWindow_);
	}

	void readInput()
	{
		int ch;
		while ((ch = wgetch(debugWindow_)) != '\n' and ch != '\r')
		{
			/* ncurses nodelay, wgetch returns ERR when no input happens*/
			if (ch == ERR) { return; }

			if (inputBufIndex < (sizeof(inputBuf) - 1U))
			{
				waddch(debugWindow_, ch);
				inputBuf[inputBufIndex] = ch;
				++inputBufIndex;
			}
		}
		inputBuf[inputBufIndex] = '\0';
		inputBufIndex = 0U;
	}



private:
	WINDOW* debugWindow_;
	cli::CLI cli_;
	char inputBuf[DEBUG_WINDOW_COLS];
	std::uint8_t inputBufIndex{0U};
	std::vector<std::string> history_{};
	std::size_t historyIndex_{0U};
};

} // namespace tui