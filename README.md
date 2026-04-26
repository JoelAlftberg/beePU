# beePU
A 16-bit emulated CPU implementing a custom instruction set (bIS), with an accompanying assembler (basm) for compiling bIS assembly into binary.

## Features
* CLI with built-in debugger
* Custom instruction set with 64 instructions
* Custom assembler for the instruction set
* Memory banking
* Stack 

## Specifications
* little-endian
* 16-bit address space
* 16 registers
* 64kB Memory

## Coming features
* More instructions (MUL, DIV, SHL, SHR, ...)
* BIOS/Bootloader
* Display
* I/O and interrupts

## Building

Requires GCC 13 or later (C++23)  

```bash
make
```

## Usage

### Running a binary
To get the emulator to run a binary, just specify the file after the executable  
```bash
./beePU <program.bin>
```

## CLI
The user is able control the emulator by the built-in CLI and a set of commands.

### Commands:
* `run` - Run the emulator
* `step <n>` - Step n instructions, defaults to 1
* `break <addr>` - Set a breakpoint at a specified address
* `get` - Get information about registers, memory, flags or a specific setting
* `set` - Set the value of a register, memory address, flag or a specific setting
* `status` - Show an overview of all settings
* `help <cmd>` - Show a list of available commands or description of a specific command
* `exit` - Exit the emulator

## [Documentation](documentation/index.md)
