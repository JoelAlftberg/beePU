# bIS - beePU Instruction Set

The Instruction set for the beeCPU is called bIS which stands for *beePU Instruction Set*  
The instructions are two bytes long, the layout of the opcodes are explained in detail below.

## Assembly syntax
The source code must start with `.org` and the origin address of the program.  
Labels must be on their own lines and must end with `:`  
Comments start with `;`  

## Field layout

### Format selector bits  
The first two bits of the opcode `[15,14]` is used to select which format the instruction is, these are the available formats: 
* `00` - Two-register operations
* `01` - Immediate operations
* `10` - Jump operations
* `11` - Single-register operations

# Instructions

## ADD
`00 0000 -- xxxx xxxx`  

The ADD instruction adds the value of the source register to the value stored in the destination register.   
* `[7:4]` - Source register (value being added)
* `[3:0]` - Destination register (where result is to be stored)

## AND
`00 0100 -- xxxx xxxx`  

The AND instruction does a logical AND operation on the source and destination registers.  
The result of the operation is stored in the destination register.  
Sets the Z Flag in the flag register if the result is zero and the Sign flag if the MSB is 1.

* `[7:4]` - Source register
* `[3:0]` - Destination register (where result is to be stored)

## BEQ
`10 0001 xxxxxxxxxx`  

The BEQ instruction jumps to the specified *signed* offset if the Z flag is set.  
The offset has the range -512 to +511 bytes.
* `[9:0]` - Signed offset in bytes to jump on branch

## BGT
`10 0011 xxxxxxxxxx`  

The BGT instruction jumps to the specified *signed* offset if **neither** the Z flag or N flag is set.  
The offset has the range -512 to +511 bytes.
* `[9:0]` - Signed offset in bytes to jump on branch

## BLT
`10 0100  xxxxxxxxxx`  

The BLT instruction jumps to the specified *signed* offset if the N flag is set.  
The offset has the range -512 to +511 bytes.
* `[9:0]` - Signed offset in bytes to jump on branch

## BNE
`10 0010  xxxxxxxxxx`  

The BNE instruction jumps to the specified *signed* offset if the Z flag is **not** set.  
The offset has the range -512 to +511 bytes.
* `[9:0]` - Signed offset in bytes to jump on branch

## BNK
`01 0010 -- ---- ----`

Switches memory bank to the bank specified in the Register `R10` (Bank Register).  

## HLT
`11 0000 -- ---- ----`  

The HLT instruction halts the operation of the CPU permanently.  
* Bits `[7:0]` are ignored in this opcode.

## JMP
`11 0011 -- xxxx ----`  

The JMP instruction jumps (sets the PC) to the address stored in the target register  
* `[7:4]` - Target register
* Bits `[3:0]` are ignored in this opcode.

## JMPI
`10 0000 xxxxxxxxxx`  
The JMPI instruction jumps (sets the PC) based on the offset specified.  
* `[9:0]` - Signed offset in bytes to jump

## LDA
`00 0010 -- xxxx xxxx`  

The LDA instruction loads the value at the *address* which the source register holds to the destination register.  
* `[7:4]` -  Source register, containing an address
* `[3:0]` - Destination register (where the data is to be stored)

## LDI
`11 0100 -- xxxx xxxx`  
The LDA instruction loads a 4 bit value immediately to a destination register.  
* `[7:4]` -  Destination register
* `[3:0]` - Value to store

## LLI
`01 0001 xx xxxxxxxx`  

The LLI instruction loads an 8-bit value immediately to a the lower 8-bits of a register.  
Only works for the R0-R3 registers, use MOV to move the value after loading it.

* `[9:8]` -  Destination register 
* `[7:0]` - Value to store

## LUI
`01 0000 xx xxxxxxxx`  

The LUI instruction loads an 8-bit value immediately to a the upper 8-bits of a register.  
Only works for the R0-R3 registers, use MOV to move the value after loading it.

* `[9:8]` -  Destination register 
* `[7:0]` - Value to store

## MOV
`00 0001 -- xxxx xxxx`  
The MOV instruction loads the value from the source to the destination register.  

* `[7:4]` - Source register (Register to fetch from)
* `[3:0]` - Destination register (where data is to be stored)

## NOP
`11 0001 -- ---- ----`  

The NOP instruction skips one cycle of processing.  
* Bits `[7:0]` are ignored in this opcode.

## NOT
`11 0010 -- xxxx ----`

The NOT instruction performs a logical NOT operation on the register specified, which inverts all it's bits.  
* `[7:4]` - Target register
* Bits `[3:0]` are ignored in this opcode.

## OR
`00 0011 -- xxxx xxxx`  

The OR instruction does a logical OR operation on the source and destination registers.  
The result of the operation is stored in the destination register.  
Sets the Z Flag in the flag register if the result is zero and the Sign flag if the MSB is 1.

* `[7:4]` - Source register
* `[3:0]` - Destination register (where result is to be stored)

## STA
`00 0101 -- xxxx xxxx`

The STA instruction reads value from a register and stores it at the address in the destination register.  
* `[7:4]` - Source register
* `[3:0]` - Destination register (containing address to store value at)

## SUB
`00 0110 -- xxxx xxxx`  

The SUB instruction subtractes the value of the source register from the value stored in the destination register.   
* `[9:8]`  - Unused
* `[7:4]` - Source register (value to subtract)
* `[3:0]` - Destination register (containing value to subtract from)

## XOR
`00 0111 -- xxxx xxxx`  

The XOR instruction does a logical XOR operation on the source and destination registers.  
The result of the operation is stored in the destination register.  
Sets the Z Flag in the flag register if the result is zero and the Sign flag if the MSB is 1.

* `[7:4]` - Source register
* `[3:0]` - Destination register (where result is to be stored)