LIB_PATH := ../lib

.PHONY: default build clean

default: clean build

build:
	mkdir -p build
	g++ -g -Wall -Werror -std=c++23 source/main.cpp source/assembler.cpp -I include -I $(LIB_PATH) -o build/basm 

clean:
	rm -rf build/