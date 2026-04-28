.PHONY: default build run

default: clean build 

build:
	mkdir -p build
	g++ -Wall -Werror -std=c++23 source/main.cpp source/cpu/cpu.cpp source/cpu/instructions.cpp source/cli/cli.cpp source/memory/memorycontroller.cpp -Iinclude -Ilib -lncurses -o build/beePU

clean:
	rm -rf build