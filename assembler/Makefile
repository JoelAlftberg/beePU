.PHONY: default build clean

default: build 

build:
	mkdir -p build
	g++ -g -Wall -Werror -std=c++23 main.cpp source/assembler.cpp -I include -o build/basm 

clean:
	rm -rf build/