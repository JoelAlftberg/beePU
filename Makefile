.PHONY: default build run

default: clean build 

build:
	mkdir -p build
	g++ main.cpp source/cpu/cpu.cpp source/cpu/instructions.cpp -I include -o build/beePU

clean:
	rm -rf build