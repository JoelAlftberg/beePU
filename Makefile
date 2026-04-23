.PHONY: default build run

default: build run

build:
	g++ main.cpp source/cpu/cpu.cpp source/cpu/instructions.cpp -I include -o build/beePU

run:
	build/beePU