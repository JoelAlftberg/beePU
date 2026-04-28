#pragma once

#include "tui/activepane.h"
#include "arch.h"
#include "cli/cli.h"
#include "cpu/cpu.h"
#include "utils.h"

//#include "ncurses/ncurses.h"
#include <ncurses.h>

#include <optional>
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

	struct Output
	{	
		std::optional<std::string> memoryArg;
	};

	Output tick(ActivePane activePane)
	{
		Output output{};

		if (activePane != ActivePane::Debugger or !readInput()) { return output; }

		std::string inputStr = inputBuf;
		cli::Input input{cli_.readInput(inputStr)};

		/* ignore the status command in cli, since register/memory view already shows it*/
		if ("status" == input.command)
		{
			resetPrompt();
			return output;
		}

		cli::Output cliOutput{cli_.evaluateInput(input)};

		werase(debugWindow_);
		box(debugWindow_, 0, 0);
		mvwprintw(debugWindow_, 0, 2, "Debug");

		if ("get" == input.command and input.arguments.size() > 1 and "mem" == input.arguments[0])
		{
			try
			{
				output.memoryArg = input.arguments[1];
			}
			catch (const std::exception& e) {}
		}
		else
		{
			printOutput(cliOutput.lines);
		}
		
		resetPrompt();

		return output;
	}

	void resetPrompt()
	{
		mvwprintw(debugWindow_, 1, 1, "> ");
		mvwhline(debugWindow_, 1, 3, ' ', DEBUG_WINDOW_COLS);
	}

	void printOutput(std::vector<std::string> lines)
	{
		std::uint8_t maxLines{static_cast<uint8_t>(getmaxy(debugWindow_) - 1)};
		std::uint8_t row{2U};

		for (std::string line : lines)
		{
			if(row >= maxLines) { return; }
			mvwaddnstr(debugWindow_, row, 1U, line.c_str(), DEBUG_WINDOW_COLS);
			row++;
		}
	}

	bool readInput()
	{
		int ch;
		while ((ch = wgetch(debugWindow_)) != '\n' and ch != '\r')
		{
			/* ncurses nodelay, wgetch returns ERR when no input is given */
			if (ch == ERR) { return false; }

			switch(ch)
			{
				case KEY_BACKSPACE:
				{
					if (inputBufIndex > 0)
					{
						inputBuf[inputBufIndex--] = '\0';
						mvwaddch(debugWindow_, 1, 3 + inputBufIndex, ' ');
						wmove(debugWindow_, 1, 3 +inputBufIndex);
					}
				}

			}

			if (ch >= 32 and ch < 127 and (inputBufIndex < (sizeof(inputBuf) - 1U)))
			{
				waddch(debugWindow_, ch);
				inputBuf[inputBufIndex] = ch;
				++inputBufIndex;
			}
		}
		inputBuf[inputBufIndex] = '\0';
		inputBufIndex = 0U;
		
		return true;
	}

private:
	WINDOW* debugWindow_;
	cli::CLI cli_;
	char inputBuf[DEBUG_WINDOW_COLS];
	std::uint8_t inputBufIndex{0U};
	std::vector<std::string> history_{};
};

} // namespace tui