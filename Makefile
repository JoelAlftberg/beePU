.PHONY: default build run

default: clean build 

build:
	mkdir -p build
	g++ main.cpp -Wall -Werror -std=c++23 source/cpu/cpu.cpp source/cpu/instructions.cpp source/cli/cli.cpp -I include -I lib -o build/beePU

clean:
	rm -rf build