#pragma once

#include "tui/activepane.h"
#include "arch.h"
#include "cpu/cpu.h"
#include "tui/debugger.h"
#include "tui/register.h"
#include "tui/memory.h"

//#include "ncurses/ncurses.h"
#include <ncurses.h>

#include <cstdlib>
#include <iostream>

namespace tui
{



class Screen
{

public:

	explicit Screen(cpu::CPU& cpu)
		: screen_{initscr()}
		, displayWindow_{newwin(DISPLAY_PANE_LINES, DISPLAY_PANE_COLS, 0, 0)}
		, statusWindow_{newwin(STATUS_PANE_LINES, RIGHT_PANE_COLS, 0, DISPLAY_PANE_COLS)}
		, memoryWindow_{newwin(MEMORY_PANE_LINES, RIGHT_PANE_COLS, STATUS_PANE_LINES, DISPLAY_PANE_COLS)}
		, debuggerWindow_{newwin(TOTAL_HEIGHT - DISPLAY_PANE_LINES , DISPLAY_PANE_COLS,  DISPLAY_PANE_LINES, 0)}
		, debugger_{debuggerWindow_, cpu}
		, registerPane_{statusWindow_, cpu}
		, memoryPane_{memoryWindow_, cpu}
	{
		getmaxyx(screen_, lines_, cols_);
		if(lines_ < TOTAL_HEIGHT or cols_ < TOTAL_WIDTH)
		{
			endwin();
			std::cout << "Terminal too small" << "\n";
			std::exit(1);
		}
		setup();
	}

	void setup()
	{
		noecho();
		
		box(displayWindow_, 0, 0);
		mvwprintw(displayWindow_, 0, 2, "beePU");

		box(statusWindow_, 0, 0);
		mvwprintw(statusWindow_, 0, 2, "State:");

		box(memoryWindow_, 0, 0);
		mvwprintw(memoryWindow_, 0, 2, "Memory:");

		box(debuggerWindow_, 0, 0);
		mvwprintw(debuggerWindow_, 0, 2, "Debug");

		keypad(debuggerWindow_, TRUE);
		curs_set(2);
		mvwprintw(debuggerWindow_, 1, 1, "> ");
		nodelay(debuggerWindow_, TRUE);
	}

	void tick()
	{
		Debugger::Output debugOutput{debugger_.tick(activePane_)};
		registerPane_.tick();
		memoryPane_.tick(debugOutput);
		
	}

	~Screen()
	{
		endwin();
	}

	void update()
	{
		wnoutrefresh(displayWindow_);
		wnoutrefresh(memoryWindow_);
		wnoutrefresh(statusWindow_);
		wnoutrefresh(debuggerWindow_);
		
		doupdate();
	}

private:
	int lines_, cols_;
	WINDOW* screen_;
	WINDOW* displayWindow_;
	WINDOW* statusWindow_;
	WINDOW* memoryWindow_;
	WINDOW* debuggerWindow_;
	Debugger debugger_;
	RegisterPane registerPane_;
	MemoryPane memoryPane_;
	ActivePane activePane_{ActivePane::Debugger};

};

} // namespace tui