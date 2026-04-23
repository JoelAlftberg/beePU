# beePU
A 16-bit emulated CPU implementing a custom instruction set (bIS), with an accompanying assembler (basm) for compiling bIS assembly into binary.

Features
* 16 registers
* 64kB RAM
* Custom instruction set with 64 instructions

Specification
* little-endian
* 16-bit address space

Coming features
* CLI interface
* More instructions (MUL, DIV, ...)
* Stack support
* Memory banking
* Bootloader

## Building

Requires GCC 13 or later (C++23)  

```bash
make
```

## Usage

### Running a binary
To get the emualator to run a binary, just specify the file after the executable  
```bash
./beePU <program.bin>
```

## CLI
The user is able control the emulator by the built-in CLI and a set of commands.

### Commands:
* `run` - Run the emulator
* `pause`- Pause the emulator
* `step <n>` - Step n instructions, defaults to 1
* `break <addr>` - Set a breakpoint at a specified address
* `get` - Get information about registers, memory, flags or a specific setting
* `set` - Set the value of a register, memory address, flag or a specific setting
* `status` - Show an overview of all settings
* `clear` - Clear the terminal
* `help <cmd>` - Show a list of available commands or description of a specific command
* `exit` - Exit the emulator

